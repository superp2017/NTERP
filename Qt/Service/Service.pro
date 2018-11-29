#-------------------------------------------------
#
# Project created by QtCreator 2018-05-11T15:13:28
#
#-------------------------------------------------

QT       -= gui

TARGET = Service
TEMPLATE = lib
QT += printsupport
QT += network


DEFINES += SERVICE_LIBRARY

QT       += axcontainer

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


UI_DIR = ../../../temp/ui/ERPService
OBJECTS_DIR = ../../../temp/obj/ERPService
MOC_DIR = ../../../temp/moc/ERPService



include(../pub.pri)




SOURCES += service.cpp \
    http.cpp \
    userservice.cpp \
    orderservice.cpp \
    customerservice.cpp \
    unitservice.cpp \
    excelservice.cpp \
    materialservice.cpp \
    supplierservice.cpp \
    accountservice.cpp \
    goodsService.cpp \
    platingservice.cpp \
    goodsoutrecordservice.cpp \
    printer.cpp \
    notification.cpp

HEADERS += service.h\
        service_global.h \
    http.h \
    userservice.h \
    orderservice.h \
    customerservice.h \
    unitservice.h \
    excelservice.h \
    materialservice.h \
    supplierservice.h \
    accountservice.h \
    goodsService.h \
    platingservice.h \
    goodsoutrecordservice.h \
    printer.h \
    notification.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
