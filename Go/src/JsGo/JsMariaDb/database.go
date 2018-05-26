package JsMariaDb

import (
	. "JsGo/JsLogger"
	"database/sql"
	"fmt"

	_ "github.com/go-sql-driver/mysql"
)

type JsDb struct {
	Host     string
	Dbname   string
	Username string
	Password string
	Db       *sql.DB
}

func NewJsDb(host, db, username, password string) *JsDb {
	h := &JsDb{host, db, username, password, nil}
	var err error
	h.Db, err = sql.Open("mysql", fmt.Sprintf("%s:%s@tcp(%s:3306)/%s", username, password, host, db)) //"user:password@/dbname")
	if err != nil {
		Error(err.Error())
		return nil
	}
	return h
}
