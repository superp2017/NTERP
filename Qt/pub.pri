
ROOT = "D:/workspace/libs/boost_1_59_0"

INCLUDEPATH += $${ROOT}/boost
QMAKE_INCDIR += $${ROOT}/boost
QMAKE_LIBDIR += $${ROOT}/lib64

LIBS += boost_system-vc140-mt-1_59.lib
LIBS += boost_thread-vc140-mt-1_59.lib

DESTDIR = ../../bin
QMAKE_LIBDIR += ../../bin

QMAKE_CXXFLAGS += -wd4100  -wd4005 -wd4503 -wd4101 -wd4100
QMAKE_CXXFLAGS += -wd4996  -wd4522 -wd4189 -wd4819
