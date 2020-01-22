
#INCLUDEPATH += $${ROOT}/boost
#QMAKE_INCDIR += $${ROOT}/boost
#QMAKE_LIBDIR += $${ROOT}/lib64
#LIBS += boost_system-vc140-mt-1_59.lib
#LIBS += boost_thread-vc140-mt-1_59.lib
#ROOT = "../libs/boost_1_67_vs2017"


ROOT = "D:/workspace/libs/boost_1_67_vs2017"

DEPENDPATH   += D:/workspace/libs/boost_1_67_vs2017/include/boost-1_67
INCLUDEPATH  += D:/workspace/libs/boost_1_67_vs2017/include/boost-1_67
QMAKE_INCDIR += D:/workspace/libs/boost_1_67_vs2017/include/boost-1_67
QMAKE_LIBDIR += D:/workspace/libs/boost_1_67_vs2017/lib

INCLUDEPATH += $${ROOT}/include/boost-1_67
QMAKE_INCDIR += $${ROOT}/include/boost-1_67
QMAKE_LIBDIR += $${ROOT}/lib

LIBS += libboost_system-vc141-mt-x64-1_67.lib
LIBS += libboost_thread-vc141-mt-x64-1_67.lib


DESTDIR = ../../bin
QMAKE_LIBDIR += ../../bin

UI_DIR = ../../../temp/ui/ERP
OBJECTS_DIR = ../../../temp/obj/ERP
MOC_DIR = ../../../temp/moc/ERP

QMAKE_CXXFLAGS += -wd4100  -wd4005 -wd4503 -wd4101 -wd4100
QMAKE_CXXFLAGS += -wd4996  -wd4522 -wd4189 -wd4819

#QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
