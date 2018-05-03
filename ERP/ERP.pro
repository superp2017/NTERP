#-------------------------------------------------
#
# Project created by QtCreator 2018-04-27T16:17:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ERP
TEMPLATE = app


DESTDIR = ../bin
QMAKE_LIBDIR += ../bin

QMAKE_CXXFLAGS += -wd4100  -wd4005 -wd4503 -wd4101
QMAKE_CXXFLAGS += -wd4996  -wd4522 -wd4189 -wd4819
UI_DIR = ../../temp/ui/ERP
OBJECTS_DIR = ../../temp/obj/ERP
MOC_DIR = ../../temp/moc/ERP

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    datacenter.cpp \
    customtabstyle.cpp \
    ordermanager.cpp \
    personmanager.cpp \
    storemanager.cpp \
    systemmanager.cpp \
    formcenter.cpp

HEADERS  += mainwindow.h \
    login.h \
    datacenter.h \
    customtabstyle.h \
    ordermanager.h \
    personmanager.h \
    storemanager.h \
    systemmanager.h \
    formcenter.h

FORMS    += mainwindow.ui \
    login.ui \
    ordermanager.ui \
    personmanager.ui \
    storemanager.ui \
    systemmanager.ui \
    formcenter.ui

RESOURCES += \
    res.qrc
