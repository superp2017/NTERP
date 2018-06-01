// Copyright 2015 The Go Authors. All rights reserved.
// Author : mengzhaofeng
// Create Date : 2015/11/05
//
// 功能：　jslogger 日志模块，提供Info,Warn,Error等级别
package JLogger

import (
	"errors"
	"fmt"
	"io"
	"log"
	"os"
	"runtime"
	"runtime/debug"
	"sync"
	"time"

	"JGo/JConfig"
)

// 日志常用参数
const (
	// Bits or'ed together to control what's printed.
	// There is no control over the order they appear (the order listed
	// here) or the format they present (as described in the comments).
	// The prefix is followed by a colon only when Llongfile or Lshortfile
	// is specified.
	// For example, flags Ldate | Ltime (or LstdFlags) produce,
	//	2009/01/23 01:23:23 message
	//  while flags Ldate | Ltime | Lmicroseconds | Llongfile produce,
	//	2009/01/23 01:23:23.123123 /a/b/c/d.go:23: message
	CT_Date         = 1 << iota // the date in the local time zone: 2009/01/23
	CT_Time                     // the time in the local time zone: 01:23:23
	CT_Microseconds             // microsecond resolution: 01:23:23.123123.  assumes Ltime.
	//	Llongfile                     // full file name and line number: /a/b/c/d.go:23
	CT_ShortFile                     // final file name element and line number: d.go:23. overrides Llongfile
	CT_UTC                           // if Ldate or Ltime is set, use UTC rather than the local time zone
	CT_StdFlags  = CT_Date | CT_Time // initial values for the standard logger
)

// 日志级别参数
const (
	CT_Info = 1 << iota
	CT_Warn
	CT_Error
	ct_none
)

// 日志信息结构
type ST_Logger struct {
	mu      sync.Mutex // ensures atomic writes; protects the following fields
	prefix  string     // prefix to write at beginning of each line
	flag    int        // properties
	out     io.Writer  // destination for output
	buf     []byte     // for accumulating text to write
	level   int        // log level info/warn/error
	std     bool       //　stdout
	maxSize int
	curPos  int
	file    *os.File
	name    string
}

// 日志句柄缓存
var g_LogMap map[string]*ST_Logger

// 日志锁
var g_LoggerLock sync.Mutex

// 通用日志
var g_ComLogger *ST_Logger = nil

// 控制台输出句柄
var g_Std = New(os.Stderr, "", "", CT_StdFlags, CT_Info)

//
func init() {
	g_LogMap = make(map[string]*ST_Logger)
	go loggerCoolie()

	var ok bool
	g_ComLogger, ok = GetComLogger()
	if !ok {
		log.Fatalln("GetComLogger error...")
	}
}

func Info(format string, v ...interface{}) {
	g_ComLogger.Info(format, v...)
}

func Warn(format string, v ...interface{}) {
	g_ComLogger.Warn(format, v...)
}

func Error(format string, v ...interface{}) {
	g_ComLogger.Error(format, v...)
}

func TraceException() {
	if err := recover(); err != nil {
		Error(string(debug.Stack()))
	}
}

func Console(std bool) {
	g_ComLogger.Console(std)
}

func ErrorLog(format string, v ...interface{}) error {
	i := fmt.Sprintf(format, v...)
	g_ComLogger.Error(i)
	return errors.New(i)
}

func loggerCoolie() {
	for {
		time.Sleep(time.Second * 10)

		level := check_level()

		if level == "Info" {
			set_logs_level(CT_Info)
		} else if level == "Warn" {
			set_logs_level(CT_Warn)
		} else if level == "Error" {
			set_logs_level(CT_Error)
		} else if level == "None" {
			set_logs_level(ct_none)
		}

	}

}

func check_level() string {
	userFile := "./conf/log.ini"
	fin, err := os.Open(userFile)
	defer fin.Close()
	if err != nil {
		log.Printf("./conf/log.ini open error : %s\n", err.Error())
		return ""
	}

	buf := make([]byte, 8)
	n, e := fin.Read(buf)
	if e != nil {
		log.Printf("./conf/log.ini read error : %s\n", e.Error())
		return ""
	}

	for n > 0 && buf[n-1] == '\n' {
		n--
	}

	return string(buf[0:n])

}

func set_logs_level(level int) {
	for _, v := range g_LogMap {

		v.Level(level)
	}

	if g_ComLogger != nil {
		g_ComLogger.Level(level)
	}
}

// 日志清理
func ClearLogger() {

	for _, value := range g_LogMap {
		value.file.Close()
	}

	if g_ComLogger != nil {
		g_ComLogger.file.Close()
	}
}

// 初始化相关日志
func initLogger(str string) (*ST_Logger, bool) {

	g_LoggerLock.Lock()
	defer g_LoggerLock.Unlock()
	file, err := os.OpenFile(str, os.O_APPEND|os.O_WRONLY, 0666)
	if err != nil {

		file, err := os.Create(str)
		if err != nil {
			log.Fatalln("init logger failed. path= " + str + "err: " + err.Error())
		}

		logger := New(file, str, "", CT_Date|CT_Time|CT_Microseconds|CT_ShortFile, CT_Info)

		g_LogMap[str] = logger

		return logger, true
	}

	return New(file, str, "", CT_Date|CT_Time|CT_Microseconds|CT_ShortFile, CT_Info), true
}

// 获取文件名对应的日志句柄
func GetLogger(str string) (*ST_Logger, bool) {
	logger, ok := g_LogMap[str]
	if !ok {
		logger, ok := initLogger(str)
		if !ok {
			return nil, ok
		}

		return logger, ok
	}
	return logger, ok
}

// 获取通用日志句柄
func GetComLogger() (*ST_Logger, bool) {
	if g_ComLogger != nil {
		return g_ComLogger, true
	}

	path, err := JConfig.GetConfigString([]string{"Logger", "Logger"})
	if err != nil {
		return nil, false
	}
	var ok bool
	g_ComLogger, ok = initLogger("./log/" + path)
	if !ok {
		return nil, ok
	}

	console, e := JConfig.GetConfigString([]string{"Logger", "Console"})
	if e != nil {
		console = "false"
	}
	if console == "true" {
		Console(true)
	} else {
		Console(false)
	}
	return g_ComLogger, ok
}

// 新见日志句柄
func New(lout io.Writer, lname string, prefix string, lflag int, llevel int) *ST_Logger {
	msize, err := JConfig.GetConfigInteger([]string{"Logger", "MaxSize"})
	if err != nil {
		return nil
	}

	return &ST_Logger{out: lout, name: lname, prefix: prefix, flag: lflag, level: llevel, maxSize: msize, curPos: 0}
}

// Cheap integer to fixed-width decimal ASCII.  Give a negative width to avoid zero-padding.
func itoa(buf *[]byte, i int, wid int) {
	// Assemble decimal in reverse order.
	var b [20]byte
	bp := len(b) - 1
	for i >= 10 || wid > 1 {
		wid--
		q := i / 10
		b[bp] = byte('0' + i - q*10)
		bp--
		i = q
	}
	// i < 10
	b[bp] = byte('0' + i)
	*buf = append(*buf, b[bp:]...)
}

// 开关控制台
func (l *ST_Logger) Console(std bool) {
	l.std = std
}

// 设置日志级别
func (l *ST_Logger) Level(level int) {
	l.level = level
}

func (l *ST_Logger) formatHeader(buf *[]byte, t time.Time, file string, line int) {
	*buf = append(*buf, l.prefix...)
	if l.flag&CT_UTC != 0 {
		t = t.UTC()
	}
	if l.flag&(CT_Date|CT_Time|CT_Microseconds) != 0 {
		if l.flag&CT_Date != 0 {
			year, month, day := t.Date()
			itoa(buf, year, 4)
			*buf = append(*buf, '-')
			itoa(buf, int(month), 2)
			*buf = append(*buf, '-')
			itoa(buf, day, 2)
			*buf = append(*buf, ' ')
		}
		if l.flag&(CT_Time|CT_Microseconds) != 0 {
			hour, min, sec := t.Clock()
			itoa(buf, hour, 2)
			*buf = append(*buf, ':')
			itoa(buf, min, 2)
			*buf = append(*buf, ':')
			itoa(buf, sec, 2)
			if l.flag&CT_Microseconds != 0 {
				*buf = append(*buf, ':')
				itoa(buf, t.Nanosecond()/1e3, 6)
			}
			*buf = append(*buf, ' ')
		}
	}
	if l.flag&CT_ShortFile != 0 {
		short := file
		for i := len(file) - 1; i > 0; i-- {
			if file[i] == '/' {
				short = file[i+1:]
				break
			}
		}
		file = short

		*buf = append(*buf, file...)
		*buf = append(*buf, '(')
		itoa(buf, line, -1)
		*buf = append(*buf, ") "...)
	}
}

// Output writes the output for a logging event.  The string s contains
// the text to print after the prefix specified by the flags of the
// Logger.  A newline is appended if the last character of s is not
// already a newline.  Calldepth is used to recover the PC and is
// provided for generality, although at the moment on all pre-defined
// paths it will be 2.
func (l *ST_Logger) Output(calldepth int, s string) error {
	now := time.Now() // get this early.
	var file string
	var line int
	l.mu.Lock()
	defer l.mu.Unlock()
	if l.flag&CT_ShortFile != 0 {
		// release lock while getting caller info - it's expensive.
		l.mu.Unlock()
		var ok bool
		_, file, line, ok = runtime.Caller(calldepth)
		if !ok {
			file = "???"
			line = 0
		}
		l.mu.Lock()
	}
	l.buf = l.buf[:0]
	l.formatHeader(&l.buf, now, file, line)
	l.buf = append(l.buf, s...)
	if len(s) == 0 || s[len(s)-1] != '\n' {
		l.buf = append(l.buf, '\n')
	}

	l.curPos += len(l.buf)

	_, err := l.out.Write(l.buf)
	if l.std {
		g_Std.out.Write(l.buf)
	}

	if l.curPos > l.maxSize {
		// fmt.Printf("curPos = %d, maxSize = %d\n", l.curPos, l.maxSize)

		l.swapLogger()
	}

	return err
}

// 交换日志文件，当日志文件>１ＭＢ时，将被调用
func (l *ST_Logger) swapLogger() {

	l.file.Close()

	nName := fmt.Sprintf("%s_%d-%d-%d_%d:%d:%d", l.name, time.Now().Year(), time.Now().Month(), time.Now().Day(), time.Now().Hour(), time.Now().Minute(), time.Now().Second())

	err := os.Rename(l.name, nName)

	l.curPos = 0

	l.file, err = os.OpenFile(l.name, os.O_APPEND|os.O_WRONLY, 0666)
	if err != nil {
		l.file, err = os.Create(l.name)
		if err != nil {
			log.Fatalln("init logger failed.")
		}
	}

	l.out = l.file
}

// Info日志输出
func (l *ST_Logger) Info(format string, v ...interface{}) {
	if l.level <= CT_Info {
		l.prefix = "[INFO] "
		l.Output(3, fmt.Sprintf(format, v...))
	}
}

// Warn 日志输出
func (l *ST_Logger) Warn(format string, v ...interface{}) {
	if l.level <= CT_Warn {
		l.prefix = "[WARN] "
		l.Output(3, fmt.Sprintf(format, v...))
	}
}

// Error 日志输出
func (l *ST_Logger) Error(format string, v ...interface{}) {

	if l.level <= CT_Error {
		l.prefix = "[ERROR] "
		l.Output(3, fmt.Sprintf(format, v...))
	}
}
