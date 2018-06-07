#-------------------------------------------------
#
# Project created by QtCreator 2018-04-27T16:17:56
#
#-------------------------------------------------

QT       += core gui
QT       += network


QT       += axcontainer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ERP
TEMPLATE = app


DESTDIR = ../../bin
QMAKE_LIBDIR += ../../bin

QMAKE_CXXFLAGS += -wd4100  -wd4005 -wd4503 -wd4101 -wd4100
QMAKE_CXXFLAGS += -wd4996  -wd4522 -wd4189 -wd4819
UI_DIR = ../../../temp/ui/ERP
OBJECTS_DIR = ../../../../temp/obj/ERP
MOC_DIR = ../../../temp/moc/ERP

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


ROOT = "D:/workspace/libs/boost_1_59_0"



INCLUDEPATH += $${ROOT}/boost
QMAKE_INCDIR += $${ROOT}/boost
QMAKE_LIBDIR += $${ROOT}/lib64

LIBS += boost_system-vc140-mt-1_59.lib
LIBS += boost_thread-vc140-mt-1_59.lib

RC_FILE = app.rc


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    datacenter.cpp \
    customtabstyle.cpp \
    ordermanager.cpp \
    personmanager.cpp \
    storemanager.cpp \
    systemmanager.cpp \
    formcenter.cpp \
    ordertable.cpp \
    dialogorderdetail.cpp \
    dialogneworder.cpp \
    dialognewcustom.cpp \
    loadding.cpp \
    nsysconfig.cpp \
    dialognewmateriel.cpp \
    dialognewunit.cpp \
    dialogmodprice.cpp \
    dialogorderprint.cpp \
    dialognewuser.cpp \
    usertable.cpp \
    dialoguserprint.cpp \
    formcustommanage.cpp \
    formsuppliermanage.cpp \
    formunitmanage.cpp \
    formwarehousemanage.cpp \
    m_tablewidget.cpp \
    dialogmaterialmanage.cpp \
    dialognewsupplier.cpp \
    formauthormanage.cpp \
    goodstable.cpp \
    dialognewgoods.cpp \
    dialoggoodsprint.cpp

HEADERS  += mainwindow.h \
    login.h \
    datacenter.h \
    customtabstyle.h \
    ordermanager.h \
    personmanager.h \
    storemanager.h \
    systemmanager.h \
    formcenter.h \
    ordertable.h \
    dialogorderdetail.h \
    dialogneworder.h \
    dialognewcustom.h \
    global.h \
    loadding.h \
    nsysconfig.h \
    dialognewmateriel.h \
    dialognewunit.h \
    dialogmodprice.h \
    dialogorderprint.h \
    dialognewuser.h \
    usertable.h \
    dialoguserprint.h \
    formcustommanage.h \
    formsuppliermanage.h \
    formunitmanage.h \
    formwarehousemanage.h \
    m_tablewidget.h \
    dialogmaterialmanage.h \
    dialognewsupplier.h \
    formauthormanage.h \
    goodstable.h \
    dialognewgoods.h \
    dialoggoodsprint.h

FORMS    += mainwindow.ui \
    login.ui \
    ordermanager.ui \
    personmanager.ui \
    storemanager.ui \
    systemmanager.ui \
    formcenter.ui \
    dialogorderdetail.ui \
    dialogneworder.ui \
    dialognewcustom.ui \
    loadding.ui \
    dialognewmateriel.ui \
    dialognewunit.ui \
    dialogmodprice.ui \
    dialogorderprint.ui \
    dialognewuser.ui \
    dialoguserprint.ui \
    formcustommanage.ui \
    formsuppliermanage.ui \
    formunitmanage.ui \
    formwarehousemanage.ui \
    dialogmaterialmanage.ui \
    dialognewsupplier.ui \
    formauthormanage.ui \
    dialognewgoods.ui \
    dialoggoodsprint.ui

RESOURCES += \
    res.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Service/release/ -lService
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Service/debug/ -lService
else:unix: LIBS += -L$$OUT_PWD/../Service/ -lService

INCLUDEPATH += $$PWD/../Service
DEPENDPATH += $$PWD/../Service

DISTFILES += \
    app.rc
