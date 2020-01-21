
ROOT = "D:/workspace/libs"

INCLUDEPATH += $${ROOT}/boost_1_59_0/boost
QMAKE_INCDIR += $${ROOT}/boost_1_59_0/boost
QMAKE_LIBDIR += $${ROOT}/boost_1_59_0/lib64


#INCLUDEPATH += ./include
#DEPENDPATH += ./include
#QMAKE_LIBDIR += ./include

#QMAKE_LIBDIR += ./lib


INCLUDEPATH += $${ROOT}/PoCo/include
QMAKE_INCDIR += $${ROOT}/PoCo/include
DEPENDPATH += $${ROOT}/PoCo/include
QMAKE_LIBDIR += $${ROOT}/PoCo/lib




LIBS += boost_system-vc140-mt-1_59.lib
LIBS += boost_thread-vc140-mt-1_59.lib

DESTDIR = ../../bin
QMAKE_LIBDIR += ../../bin

UI_DIR = ../../../temp/ui/ERP
OBJECTS_DIR = ../../../temp/obj/ERP
MOC_DIR = ../../../temp/moc/ERP

QMAKE_CXXFLAGS += -wd4100  -wd4005 -wd4503 -wd4101 -wd4100
QMAKE_CXXFLAGS += -wd4996  -wd4522 -wd4189 -wd4819

#QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
