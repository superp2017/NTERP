package JHttp

import (
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"net/http"
	"strconv"
	"time"

	"sync"

	"JGo/JCache"
	"JGo/JConfig"
	. "JGo/JLogger"
)

const (
	gET     = "GET"
	pOST    = "POST"
	oPTIONS = "OPTIONS"
	eXPIRE  = time.Minute * 60
	cLEAR   = time.Hour * 2
	sEXPIRE = 3600000000000
	// sEXPIRE = 30000000000 test
)

type Context struct {
	Count        uint64
	Handles      sync.Map
	WhiteHandles sync.Map
	ISession     bool
	Sncache      *JCache.Cache
	TLS          bool
}

type Session struct {
	Rsp http.ResponseWriter
	Req *http.Request
	Ctx *Context
}

//　回调函数声明
type NetCb func(ctx *Session)

type JContext struct {
	Context
}

type JContextTLS struct {
	Context
	Pem string
	Key string
}

var g_ctx *JContext
var g_ctx_tls *JContextTLS
var g_chan chan int
var g_get bool = true

func init() {
	g_ctx = &JContext{}
	g_ctx.Init()
	g_ctx.TLS = false
	g_ctx_tls = &JContextTLS{}
	g_ctx_tls.Init()
	g_ctx_tls.TLS = true
	g_chan = make(chan int, 1)
}

func (this *Context) Init() {
	this.Count = 0
	this.ISession = false
}

func Http(url string, cb NetCb) {
	g_ctx.Handles.Store(url, cb)
}

func WhiteHttp(url string, cb NetCb) {
	Http(url, cb)
	g_ctx.WhiteHandles.Store(url, true)
}

func WhiteHttps(url string, cb NetCb) {
	Https(url, cb)
	g_ctx_tls.WhiteHandles.Store(url, true)
}

func Https(url string, cb NetCb) {
	g_ctx_tls.Handles.Store(url, cb)
}

func (this *Context) serve(w http.ResponseWriter, r *http.Request) {

	defer TraceException()

	if r.Method == oPTIONS {
		//跨域
		w.Header().Set("Access-Control-Allow-Origin", "*")             //允许访问所有域
		w.Header().Add("Access-Control-Allow-Headers", "Content-Type") //header的类型
		w.Header().Add("Access-Control-Allow-Headers", "Js-Token")     //header的类型
		w.Header().Add("Access-Control-Allow-Headers", "Origin")
		io.WriteString(w, "")
		return
	}

	if r.Method == gET && g_get == false {
		Error("GET mothed not sported, URL:%s", r.URL.String())
		return
	}

	cb, ok := this.Handles.Load(r.URL.Path)
	if ok {
		callback, ok := cb.(NetCb)
		if ok {
			Info("url:%s match", r.URL.Path)

			//跨域
			w.Header().Set("Access-Control-Allow-Origin", "*")             //允许访问所有域
			w.Header().Add("Access-Control-Allow-Headers", "Content-Type") //header的类型
			w.Header().Add("Access-Control-Allow-Headers", "Js-Token")     //header的类型
			w.Header().Add("Access-Control-Allow-Headers", "Origin")
			w.Header().Add("Access-Control-Allow-Headers", "Authorization")
			w.Header().Add("Access-Control-Allow-Headers", "Access-Control-Expose-Headers")
			w.Header().Add("Access-Control-Expose-Headers", "Js-Token")
			w.Header().Set("content-type", "application/json") //返回数据格式是json

			session := &Session{w, r, this}

			_, ok := this.WhiteHandles.Load(r.URL.Path)
			if ok {
				//解析参数
				r.ParseForm()

				callback(session)
				return
			}

			//session 检查
			if this.ISession {

				// token, findit := this.Sncache.Get(r.RemoteAddr)
				// if !findit {
				// 	Error("Remote addr:%s session not match", r.RemoteAddr)
				// 	session.Forward("-1000", "session not found", "")
				// 	return
				// }

				js_token := r.Header.Get("Js-Token") //token 超时判断

				if js_token == "" {
					Error("Remote addr:%s session not match", r.RemoteAddr)
					session.Forward("-1000", "session not match", "")
					return
				}

				last, e := strconv.ParseInt(js_token, 10, 64)
				if e != nil {
					Error("error:%s", e.Error())
					session.Forward("-1000", e.Error(), "")
					return
				}
				now := time.Now().UnixNano()
				m := now - last

				// fmt.Printf("token = %s, m=%d, e=%d\n", js_token, m, sEXPIRE)
				if m > sEXPIRE { //session 超时
					Error("error: session expired")
					session.Forward("-1000", "session expired", "")
					return
				} else {
					w.Header().Set("Js-Token", fmt.Sprintf("%d", time.Now().UnixNano()))
				}

			}

			//解析参数
			r.ParseForm()
			callback(session)
			return
		}

	}

	Info("url:%s not match", r.URL.Path)
}

func (this *Session) MarkSession() {

	n := time.Now().UnixNano()

	this.Rsp.Header().Set("Js-Token", fmt.Sprintf("%d", n))
}

func (this *Session) IsExpired(token string) bool {

	last, e := strconv.ParseInt(token, 10, 64)
	if e != nil {
		return true
	}
	now := time.Now().UnixNano()
	m := now - last

	// fmt.Printf("token = %s, m=%d, e=%d\n", js_token, m, sEXPIRE)
	if m > sEXPIRE { //session 超时
		Error("error: session expired")
		return true
	} else {
		return false
	}
}

//获取前端给的数据
func (this *Session) GetPara(v interface{}) error {
	data := this.Req.FormValue("data")

	if data != "" {
		return json.Unmarshal([]byte(data), v)
	} else {
		return errors.New("data is nil")
	}
}

//返回给前端数据
func (this *Session) Forward(ret, msg string, data interface{}) {
	type StRet struct {
		Ret    string      //返回值
		Msg    string      //返回信息
		Entity interface{} //返回数据
	}
	retV := &StRet{}
	retV.Ret = ret
	retV.Msg = msg
	retV.Entity = data

	b, e := json.Marshal(retV)
	if e != nil {
		return
	}

	callback := this.Req.FormValue("callback")

	if callback != "" {

		io.WriteString(this.Rsp, callback+"("+string(b)+")")

		return
	}

	io.WriteString(this.Rsp, string(b))
}

func (this *Session) WriteString(data string) {
	io.WriteString(this.Rsp, data)
}

func (this *JContext) ServeHTTP(w http.ResponseWriter, r *http.Request) {

	this.serve(w, r)
}

func (this *JContextTLS) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	this.serve(w, r)
}

func enableGet() {
	g_get = true
}

func enableSession() {
	g_ctx.ISession = true
	g_ctx_tls.ISession = true
	g_ctx.Sncache = JCache.New(eXPIRE, cLEAR)
	g_ctx_tls.Sncache = JCache.New(eXPIRE, cLEAR)
}

func Run() {
	net, e := JConfig.GetConfigMap([]string{"Net"})
	if e == nil {
		if net["Session"] == "true" {
			enableSession()
		}

		if net["Get"] == "true" {
			g_get = true
		} else {
			g_get = false
		}
	} else {
		Error(e.Error())
	}

	htp, err := JConfig.GetConfigMap([]string{"Http"})
	if htp != nil && err == nil {
		go http.ListenAndServe(htp["Ip"]+":"+htp["Listen"], g_ctx)
	}

	htps, err := JConfig.GetConfigMap([]string{"Https"})

	if htps != nil && err == nil {
		go http.ListenAndServeTLS(htps["Ip"]+":"+htps["Listen"], htps["Pem"], htps["Key"], g_ctx_tls)
	}

	<-g_chan
}

func Close() {

	g_chan <- 1
}
