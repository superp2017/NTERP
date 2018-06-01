package JConfig

import (
	"encoding/json"
	"errors"
	"fmt"
	io "io/ioutil"
	"log"
)

var g_cfg map[string]interface{}

func GetConfigString(keys []string) (string, error) {

	ret := g_cfg

	L := len(keys)
	i := 0
	for {
		if i == L {
			return "", errors.New(fmt.Sprintf("no relative key %v", keys))
		}

		tmp, ok := ret[keys[i]].(map[string]interface{})
		if ok {
			ret = tmp
			i++
			continue
		} else {
			tmp, ok := ret[keys[i]].(string)

			if ok {
				return tmp, nil
			} else {
				return "", errors.New(fmt.Sprintf("no relative key %v according to string", keys))
			}
		}
	}
}

func GetConfigInteger(keys []string) (int, error) {

	ret := g_cfg

	L := len(keys)
	i := 0
	for {
		if i == L {
			return 0, errors.New(fmt.Sprintf("no relative key %v", keys))
		}

		tmp, ok := ret[keys[i]].(map[string]interface{})
		if ok {

			ret = tmp
			i++
			continue
		} else {

			tmp, ok := ret[keys[i]].(float64)

			if ok {
				return int(tmp), nil
			} else {
				return 0, errors.New(fmt.Sprintf("no relative key %v according to int", keys))
			}
		}
	}
}

func GetConfigFloat(keys []string) (float64, error) {

	ret := g_cfg

	L := len(keys)
	i := 0
	for {
		if i == L {
			return 0, errors.New(fmt.Sprintf("no relative key %v", keys))
		}

		tmp, ok := ret[keys[i]].(map[string]interface{})
		if ok {
			ret = tmp
			i++
			continue
		} else {
			tmp, ok := ret[keys[i]].(float64)
			if ok {
				return tmp, nil
			} else {
				return 0, errors.New(fmt.Sprintf("no relative key %v according to float", keys))
			}
		}
	}
}

func GetConfigMap(keys []string) (map[string]string, error) {

	ret := g_cfg

	L := len(keys)
	i := 0
	for {
		if i == L {
			return nil, errors.New(fmt.Sprintf("no relative key %v", keys))
		}

		tmp, ok := ret[keys[i]].(map[string]interface{})

		if ok && i == (L-1) {
			//tmp, ok := ret[keys[i]].(map[string]string)
			set := make(map[string]string)
			for k, v := range tmp {
				set[k] = v.(string)
			}
			return set, nil
			// if ok {
			// 	return tmp, nil
			// } else {
			// 	return nil, errors.New(fmt.Sprintf("no relative key %v according to map.", keys))
			// }

		}
		ret = tmp
		i++
	}
}

//加载配置文件
func load_config() {

	data, err := io.ReadFile("conf/config.json")

	if err != nil {
		log.Fatalln("load config file error.")
	}

	datajson := []byte(data)

	err = json.Unmarshal(datajson, &g_cfg)

	if err != nil {
		log.Fatalln(err.Error())
	}
}

//init
func init() {
	load_config()
}
