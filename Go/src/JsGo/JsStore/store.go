package JsStore

import (
	. "JsGo/JsLogger"
	"JsGo/JsStore/JsRedis"
	"JsGo/JsStore/JsSsdb"
	"errors"
)

// func Get(k string, v interface{}) error {

// 	dtio := JsSsdb.GenDtio()
// 	if dtio == nil {
// 		return errors.New("GenDtio error")
// 	}

// 	b, e := dtio.Dtio_Get(k)
// 	if e != nil {
// 		Error(e.Error())
// 		return e
// 	}

// 	e = json.Unmarshal(b, v)
// 	if e != nil {
// 		Error(e.Error())
// 		return e
// 	}

// 	return nil
// }

func Get(k string, v interface{}) error {
	return JsRedis.Redis_get(k, v)
}

func Set(k string, v interface{}) error {
	return JsRedis.Redis_set(k, v)
}

// func Set(k string, v interface{}) error {
// 	dtio := JsSsdb.GenDtio()
// 	if dtio == nil {
// 		return errors.New("GenDtio error")
// 	}

// 	b, e := json.Marshal(v)
// 	if e != nil {
// 		Error(e.Error())
// 		return e
// 	}

// 	e = dtio.Dtio_Set(k, b)
// 	if e != nil {
// 		Error(e.Error())
// 		return e
// 	}
// 	return nil
// }

func Exist(k string) (bool, error) {
	dtio := JsSsdb.GenDtio()
	if dtio == nil {
		return false, errors.New("GenDtio error")
	}

	b, e := dtio.Dtio_Exist(k)
	if e != nil {
		Error(e.Error())
		return false, e
	}
	return b, nil
}

func ShareLock(t string, k string, v interface{}) error {
	dtio := JsSsdb.GenDtio()
	if dtio == nil {
		return errors.New("GenDtio error")
	}

	_, e := dtio.Dtio_ShareLock(t, k, v)
	if e != nil {
		Error(e.Error())
		return e
	}

	return nil
}

func WriteLock(t string, k string, v interface{}) error {
	dtio := JsSsdb.GenDtio()
	if dtio == nil {
		return errors.New("GenDtio error")
	}

	e := dtio.Dtio_WriteLock(t, k, v)
	if e != nil {
		Error(e.Error())
		return e
	}

	return nil
}

func WriteBack(t string, k string, v interface{}) error {
	dtio := JsSsdb.GenDtio()
	if dtio == nil {
		return errors.New("GenDtio error")
	}

	e := dtio.Dtio_WriteBack(t, k, v)
	if e != nil {
		Error(e.Error())
		return e
	}
	return nil
}

func DirectWrite(t string, k string, v interface{}) error {
	dtio := JsSsdb.GenDtio()
	if dtio == nil {
		return errors.New("GenDtio error")
	}

	if e := dtio.Dtio_UnsafeWriteEx(t, k, v); e != nil {
		Error(e.Error())
		return e
	}
	return nil
}

func HExist(table, id string) (bool, error) {
	dtio := JsSsdb.GenDtio()
	if dtio == nil {
		return false, errors.New("GenDtio error")
	}

	b, e := dtio.Dtio_HExist(table, id)
	if e != nil {
		Error(e.Error())
		return false, e
	}
	return b, nil
}

func HSize(table string) (int64, error) {
	dtio := JsSsdb.GenDtio()
	if dtio == nil {
		return -1, errors.New("GenDtio error")
	}

	s, e := dtio.Dtio_HSize(table)

	if e != nil {
		Error(e.Error())
		return -1, e
	}
	return s, nil
}

func HKeys(table, begin, end string, limit int64) ([]string, error) {
	dtio := JsSsdb.GenDtio()
	if dtio == nil {
		return nil, errors.New("GenDtio error")
	}

	s, e := dtio.Dtio_HKeys(table, begin, end, limit)

	if e != nil {
		Error(e.Error())
		return nil, e
	}
	return s, nil
}

func HDel(table, id string) error {
	dtio := JsSsdb.GenDtio()
	if dtio == nil {
		return errors.New("GenDtio error")
	}
	if e := dtio.Dtio_HDel(table, id); e != nil {
		Error(e.Error())
		return e
	}
	return nil
}

func Del(table string) error {
	dtio := JsSsdb.GenDtio()
	if dtio == nil {
		return errors.New("GenDtio error")
	}
	if er := dtio.Dtio_Del(table); er != nil {
		Error(er.Error())
		return er
	}
	return nil
}

func CurUnLock() []*JsSsdb.ST_SData {
	dtio := JsSsdb.GenDtio()
	if dtio == nil {
		return nil
	}

	return dtio.Dtio_CurrentUnlock()

}
