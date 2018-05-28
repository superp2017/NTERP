package JsQiniu
//
//import (
//	"JsGo/JsConfig"
//	"JsGo/JsHttp"
//	. "JsGo/JsLogger"
//	"context"
//	"crypto/hmac"
//	"crypto/sha1"
//	"encoding/base64"
//	"io"
//	"log"
//	"net/http"
//	"net/url"
//	"os"
//	"time"
//
//	"qiniupkg.com/api.v7/kodo"
//)
//
//var g_client *kodo.Client
//var g_token string
//var g_AK string
//var g_SK string
//var g_scope string
//var g_bucket string
//var g_domain string
//
//func QiniuInit() {
//	JsHttp.WhiteHttps("/qiniutoken", qiniu_token)
//	JsHttp.WhiteHttps("/qiniusavesign", qiniu_save_sign)
//	JsHttp.WhiteHttps("/qiniutransferfile", transferFile)
//
//	AK, e := JsConfig.GetConfigString([]string{"Qiniu", "AK"})
//	g_AK = AK
//	if e != nil {
//		log.Fatalln(e.Error())
//	}
//
//	SK, e := JsConfig.GetConfigString([]string{"Qiniu", "SK"})
//	g_SK = SK
//	if e != nil {
//		log.Fatalln(e.Error())
//	}
//
//	g_scope, e = JsConfig.GetConfigString([]string{"Qiniu", "Scope"})
//	if e != nil {
//		log.Fatalln(e.Error())
//	}
//
//	g_domain, e = JsConfig.GetConfigString([]string{"Qiniu", "Domain"})
//	if e != nil {
//		log.Fatalln(e.Error())
//	}
//
//	g_bucket, e = JsConfig.GetConfigString([]string{"Qiniu", "Bucket"})
//	if e != nil {
//		log.Fatalln(e.Error())
//	}
//
//	kodo.SetMac(AK, SK)
//	g_client = kodo.New(0, nil)
//
//	go coolie()
//}
//
//func coolie() {
//	g_token = g_client.MakeUptoken(&kodo.PutPolicy{
//		Scope:   g_scope,
//		Expires: 86400,
//	})
//
//	for {
//		h := time.Now().Hour()
//		if h == 1 {
//			g_token = g_client.MakeUptoken(&kodo.PutPolicy{
//				Scope:   g_scope,
//				Expires: 86400,
//			})
//		}
//
//		time.Sleep(time.Hour)
//	}
//}
//
//func qiniu_token(session *JsHttp.Session) {
//	session.Forward("0", "success", g_token)
//}
//
//func qiniu_save_sign(session *JsHttp.Session) {
//	type Para_Sign struct {
//		Url     string
//		Bucket  string
//		SaveKey string
//	}
//
//	para := &Para_Sign{}
//
//	e := session.GetPara(para)
//	if e != nil {
//		Error(e.Error())
//		session.Forward("2", e.Error(), nil)
//		return
//	}
//
//	Sign := makeSaveasUrl(para.Url, g_AK, []byte(g_SK), para.Bucket, para.SaveKey)
//
//	session.Forward("0", "success", Sign)
//}
//
//func makeSaveasUrl(URL, accessKey string, secretKey []byte, saveBucket, saveKey string) string {
//
//	encodedEntryURI := base64.URLEncoding.EncodeToString([]byte(saveBucket + ":" + saveKey))
//
//	URL += "|saveas/" + encodedEntryURI
//
//	h := hmac.New(sha1.New, secretKey)
//
//	// 签名内容不包括Scheme，仅含如下部分：
//	// <Domain>/<Path>?<Query>
//
//	u, _ := url.Parse(URL)
//	io.WriteString(h, u.Host+u.RequestURI())
//
//	d := h.Sum(nil)
//	sign := accessKey + ":" + base64.URLEncoding.EncodeToString(d)
//
//	return URL + "/sign/" + sign
//
//}
//
//func transferFile(session *JsHttp.Session) {
//	type Para struct {
//		Sid   string
//		Token string
//	}
//
//	para := &Para{}
//	e := session.GetPara(para)
//	if e != nil {
//		Error(e.Error())
//		session.Forward("1", e.Error(), nil)
//		return
//	}
//	url := `http://file.api.weixin.qq.com/cgi-bin/media/get?access_token=` + para.Token + `&media_id=` + para.Sid
//
//	Info(url)
//	res, e := http.Get(url)
//	if e != nil {
//		Error(e.Error())
//		session.Forward("2", e.Error(), nil)
//		return
//	}
//	f, e := os.Create("temp/" + para.Sid)
//	if e != nil {
//		Error(e.Error())
//		session.Forward("3", e.Error(), nil)
//		return
//	}
//	io.Copy(f, res.Body)
//	f.Close()
//
//	// kodo.SetMac(CFG.Qiniu.AK, CFG.Qiniu.SK)
//	// zone := 0                // 您空间(Bucket)所在的区域
//	// c := kodo.New(zone, nil) // 用默认配置创建 Client
//
//	bucket := g_client.Bucket(g_bucket)
//	ctx := context.Background()
//
//	//httpFile := "http://e.hiphotos.baidu.com/baike/w%3D268%3Bg%3D0/sign=82b30b449052982205333ec5eff11cf6/d000baa1cd11728b8380bba1c8fcc3cec2fd2c87.jpg"
//	e = bucket.PutFile(ctx, nil, para.Sid, "temp/"+para.Sid, nil)
//	if e != nil {
//		Error(e.Error())
//
//		session.Forward("4", e.Error(), nil)
//		return
//	}
//	session.Forward("0", "success", g_domain+para.Sid)
//}
