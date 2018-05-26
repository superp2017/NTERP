package main


import (
	"JsGo/JsExit"
	"JsGo/JsHttp"
)

func exit() int {
	JsHttp.Close()
	return 0
}

func initRoute()  {
	
}


func main() {
	JsExit.RegisterExitCb(exit)
	initRoute()
	JsHttp.Run()
}
