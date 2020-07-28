package main

import (
	"JGo/JConfig"
	"JGo/JHttp"
	"net/http"
)

func Jscode2Session(session *JHttp.Session) {
	type para struct {
		Code string
	}
	st := &para{}
	if err := session.GetPara(st); err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	if st.Code == "" {
		session.Forward("1", "Code is empty!", nil)
		return
	}

	wx, err := JConfig.GetConfigMap([]string{"Wx"})

	if err != nil || wx == nil {
		session.Forward("1", "load wx config error!", nil)
		return
	}

	url := "https://api.weixin.qq.com/sns/jscode2session?appid="
	url += wx["MiniAppid"]
	url += "&secret="
	url += wx["MiniSecret"]
	url += "&js_code="
	url += st.Code
	url += "&grant_type=authorization_code"
	client := &http.Client{}
	re, err := http.NewRequest("GET", url, nil)

	if err != nil {
		session.Forward("1", err.Error(), nil)
		return
	}
	response, _ := client.Do(re)
	session.Forward("0", "success", response.Body)
	return
}
