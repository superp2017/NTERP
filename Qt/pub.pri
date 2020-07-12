

ROOT = "./../../../libs/boost_1_67_vs2017"

INCLUDEPATH += $${ROOT}/include/boost-1_67
QMAKE_INCDIR += $${ROOT}/include/boost-1_67
QMAKE_LIBDIR += $${ROOT}/lib

#LIBS += libboost_system-vc141-mt-x64-1_67.lib
#LIBS += libboost_thread-vc141-mt-x64-1_67.lib


DESTDIR = ../../JY_bin
QMAKE_LIBDIR += ../../JY_bin


CONFIG(debug, debug|release) {
#配置中间生成文件目录
    TMPD_ROOT    +=  ./../../../tmp/debug/NTERP
    RCC_DIR      +=  $${TMPD_ROOT}/rcc
    UI_DIR       +=  $${TMPD_ROOT}/ui
    OBJECTS_DIR  +=  $${TMPD_ROOT}/obj
    MOC_DIR      +=  $${TMPD_ROOT}/moc
}else{
    TMP_ROOT    +=   ./../../..//tmp/release/NTERP
    RCC_DIR      +=  $${TMP_ROOT}/rcc
    UI_DIR       +=  $${TMP_ROOT}/ui
    OBJECTS_DIR  +=  $${TMP_ROOT}/obj
    MOC_DIR      +=  $${TMP_ROOT}/moc
}


QMAKE_CXXFLAGS += -wd4100  -wd4005 -wd4503 -wd4101 -wd4100
QMAKE_CXXFLAGS += -wd4996  -wd4522 -wd4189 -wd4819

#QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
