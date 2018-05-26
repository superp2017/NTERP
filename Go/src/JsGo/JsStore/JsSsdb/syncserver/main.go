package main

import (
	"JsGo/JsExit"
)

func exit() int {

	g_log.Info("JsDBCache server begin exit...")
	StopServer()
	g_log.Info("JsDBCache server end exit...")

	return 0
}

func main() {
	JsExit.RegisterExitCb(exit)

	StartServer()

}
