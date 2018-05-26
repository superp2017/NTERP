package JsSsdb

import (
	"encoding/json"
)

type Dtio struct {
	Handle *ST_DbServer
}

func GenDtio() *Dtio {
	var dio Dtio
	dio.Handle = getHandle()

	return &dio
}

func (d *Dtio) Dtio_Get(key string) ([]byte, error) {
	return get(key)
}

func (d *Dtio) Dtio_Set(key string, data []byte) error {
	return set(key, data)
}

func (d *Dtio) Dtio_Exist(key string) (bool, error) {
	return exist(key)
}

func (d *Dtio) Dtio_ShareLock(table, id string, v interface{}) ([]byte, error) {
	return shareLock(table, id, v)
}

func (d *Dtio) Dtio_UnsafeWrite(table, id string, b []byte) error {
	return unsafeWrite(table, id, b)
}

func (d *Dtio) Dtio_UnsafeWriteEx(table, id string, v interface{}) error {
	b, e := json.Marshal(v)
	if e != nil {
		return e
	}
	return unsafeWrite(table, id, b)
}

func (d *Dtio) Dtio_WriteLock(table, id string, v interface{}) error {
	return writeLock(table, id, v)
}

func (d *Dtio) Dtio_WriteBack(table, id string, v interface{}) error {
	return writeBack(table, id, v)
}

func (d *Dtio) Dtio_HExist(table, id string) (bool, error) {
	return hExist(table, id)
}

func (d *Dtio) Dtio_HSize(table string) (int64, error) {
	return hSize(table)
}

func (d *Dtio) Dtio_HKeys(table, begin, end string, limit int64) ([]string, error) {
	return hKeys(table, begin, end, limit)
}

func (d *Dtio) Dtio_HDel(table, id string) error {
	return hDel(table, id)
}

func (d *Dtio) Dtio_Del(table string) error {
	return del(table)
}

func (d *Dtio) Dtio_ManualUnlock(table, id string) error {
	return manualUnlock(table, id)
}

func (d *Dtio) Dtio_CurrentUnlock() []*ST_SData {
	return currentUnlock()
}

func (d *Dtio) Dtio_Update(table, id string, v interface{}, mod func()) error {
	e := d.Dtio_WriteLock(table, id, v)
	if e != nil {
		return e
	}
	mod()

	return d.Dtio_WriteBack(table, id, v)

}

func (d *Dtio) Dtio_UpdateEx(table, id string, v interface{}, mod func(error)) error {
	e := d.Dtio_WriteLock(table, id, v)
	mod(e)
	if e != nil {
		return e
	}
	return d.Dtio_WriteBack(table, id, v)
}
