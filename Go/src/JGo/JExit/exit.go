package JExit

import (
	"os"
	"os/signal"
	"syscall"
)

type CB_Exit func() int

var g_ecb CB_Exit = nil

func RegisterExitCb(ecb CB_Exit) {
	g_ecb = ecb
	go exitCoolie()
}

func exitCoolie() {
	c := make(chan os.Signal)

	signal.Notify(c, syscall.SIGUSR1) //监听指定信号

	<-c //阻塞直至有信号传入

	if g_ecb != nil {
		os.Exit(g_ecb())
	}
}
