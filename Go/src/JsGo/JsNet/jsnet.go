package JsNet

import (
	"JsGo/JsConfig"
	"crypto/md5"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"log"
	"sync"
	"time"

	"github.com/astaxie/beego"
	"github.com/astaxie/beego/plugins/cors"
	"github.com/coocood/freecache"
)

type StSession struct {
	beego.Controller
}

type StRet struct {
	Ret    string
	Entity interface{}
	Msg    string
}

var g_smscache *freecache.Cache

//　回调函数声明
type NetCb func(session *StSession)

var http_cbs map[string]NetCb = make(map[string]NetCb)
var white_list map[string]NetCb = make(map[string]NetCb)
var mutex sync.Mutex

func init() {
	g_smscache = freecache.NewCache(32 * 1024 * 1024) // 32MB
}

func Http(url string, cb NetCb) {

	http_cbs[url] = cb
	beego.Router(url, &StSession{})
}

func WhiteHttp(url string, cb NetCb) {

	white_list[url] = cb

	beego.Router(url, &StSession{})
}

func HttpRedict(session *StSession, url string) {
	session.Redirect(url, 302)
}

func (session *StSession) GetPara(v interface{}) error {
	return json.Unmarshal([]byte(session.GetString("data", "")), v)
}

func (session *StSession) Forward(ret, msg string, data interface{}) {
	retV := &StRet{}
	retV.Ret = ret
	retV.Msg = msg
	retV.Entity = data

	b, e := json.Marshal(retV)
	if e != nil {
		return
	}

	callback := session.GetString("callback", "")

	if callback != "" {

		session.Ctx.WriteString(callback + "(" + string(b) + ")")
		return
	}

	session.Ctx.WriteString(string(b))
}

func Run() {
	beego.InsertFilter("*", beego.BeforeRouter, cors.Allow(&cors.Options{
		AllowAllOrigins:  true,
		AllowMethods:     []string{"GET", "POST", "PUT", "DELETE", "OPTIONS"},
		AllowHeaders:     []string{"Origin", "Authorization", "Access-Control-Allow-Origin", "Access-Control-Allow-Headers", "Content-Type", "X-Token"},
		ExposeHeaders:    []string{"Content-Length", "Access-Control-Allow-Origin", "Access-Control-Allow-Headers", "Content-Type"},
		AllowCredentials: true,
	}))

	// beego.BConfig.WebConfig.Session.SessionOn = true

	Ip, err := JsConfig.GetConfigString([]string{"Http", "Ip"})
	if err != nil {
		log.Fatalln(err)
	}

	Port, err := JsConfig.GetConfigString([]string{"Http", "Listen"})
	if err != nil {
		log.Fatalln(err)
	}
	beego.Run(Ip + ":" + Port)

}

func (session *StSession) Get() {
	session.coolie()
}

func (session *StSession) Post() {
	session.coolie()
}

func (session *StSession) Options() {
	fmt.Printf("ddd")
	session.coolie()
}

func (session *StSession) GenSession() {
	h := md5.New()
	seed := fmt.Sprintf("%d", time.Now().Nanosecond())
	h.Write([]byte(seed)) // 需要加密的字符串为 123456
	cipherStr := h.Sum(nil)
	// session.SetSession("junsie_token", hex.EncodeToString(cipherStr))
	// fmt.Printf("addr = %s\n", session.Ctx.Request.RemoteAddr)
	g_smscache.Set([]byte(session.Ctx.Request.RemoteAddr), []byte(hex.EncodeToString(cipherStr)), 3600) //一小时
}

func (session *StSession) coolie() {

	url := session.Ctx.Request.URL.Path

	mutex.Lock()
	pass, ok := white_list[url]
	mutex.Unlock()

	if ok {
		pass(session)
		return
	}

	mutex.Lock()
	cb, ok := http_cbs[url]
	mutex.Unlock()
	if !ok {
		return
	}

	// fmt.Printf("sss addr = %s\n", session.Ctx.Request.RemoteAddr)
	token, err := g_smscache.Get([]byte(session.Ctx.Request.RemoteAddr))

	if token == nil || err != nil {
		session.Forward("-1000", "no auth", nil)
	} else {
		cb(session)
	}
}

func Close() {

}

// func AppConf(conf string) {
// 	beego.AppConfigPath = conf
// 	beego.ParseConfig()
// }
