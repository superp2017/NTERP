#-------------------------------------------------
#
# Project created by QtCreator 2018-04-27T16:17:56
#
#-------------------------------------------------

QT       += core gui
QT       += network

QT	+= printsupport

QT       += axcontainer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets




TARGET = ERP
TEMPLATE = app

include(../pub.pri)


UI_DIR = ../../../temp/ui/ERP
OBJECTS_DIR = ../../../temp/obj/ERP
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


RC_FILE = app.rc


SOURCES += main.cpp\
	mainwindow.cpp \
    login.cpp \
    datacenter.cpp \
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
    dialoggoodsprint.cpp \
    dialognewgoodstype.cpp \
    formgoodstypemanage.cpp \
    datacenter.cpp \
    dialoggoodsprint.cpp \
    dialogmaterialmanage.cpp \
    dialogmodprice.cpp \
    dialognewcustom.cpp \
    dialognewgoods.cpp \
    dialognewgoodstype.cpp \
    dialognewmateriel.cpp \
    dialogneworder.cpp \
    dialognewsupplier.cpp \
    dialognewunit.cpp \
    dialognewuser.cpp \
    dialogorderdetail.cpp \
    dialogorderprint.cpp \
    dialoguserprint.cpp \
    formauthormanage.cpp \
    formcenter.cpp \
    formcustommanage.cpp \
    formgoodstypemanage.cpp \
    formsuppliermanage.cpp \
    formunitmanage.cpp \
    formwarehousemanage.cpp \
    goodstable.cpp \
    loadding.cpp \
    login.cpp \
    m_tablewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    networkmanager.cpp \
    nsysconfig.cpp \
    ordermanager.cpp \
    ordertable.cpp \
    personmanager.cpp \
    storemanager.cpp \
    systemmanager.cpp \
    usertable.cpp \
    dialognewdepartment.cpp \
    formdepartmentmanage.cpp \
    dialogordersearch.cpp \
    dialogplatingmanage.cpp \
    dialogorderproduceorout.cpp \
    goodsoutrecord.cpp \
    dialogoutgoods.cpp \
    dialoggoodsin.cpp \
    dialoggoodsoutrecordprint.cpp \
    formfinance.cpp \
    dialogprintouttable.cpp \
    formmatermanager.cpp \
    version.cpp

HEADERS  += mainwindow.h \
    login.h \
    datacenter.h \
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
    dialoggoodsprint.h \
    dialognewgoodstype.h \
    formgoodstypemanage.h \
    datacenter.h \
    dialoggoodsprint.h \
    dialogmaterialmanage.h \
    dialogmodprice.h \
    dialognewcustom.h \
    dialognewgoods.h \
    dialognewgoodstype.h \
    dialognewmateriel.h \
    dialogneworder.h \
    dialognewsupplier.h \
    dialognewunit.h \
    dialognewuser.h \
    dialogorderdetail.h \
    dialogorderprint.h \
    dialoguserprint.h \
    formauthormanage.h \
    formcenter.h \
    formcustommanage.h \
    formgoodstypemanage.h \
    formsuppliermanage.h \
    formunitmanage.h \
    formwarehousemanage.h \
    global.h \
    goodstable.h \
    loadding.h \
    login.h \
    m_tablewidget.h \
    mainwindow.h \
    networkmanager.h \
    nsysconfig.h \
    ordermanager.h \
    ordertable.h \
    personmanager.h \
    storemanager.h \
    systemmanager.h \
    usertable.h \
    dialognewdepartment.h \
    formdepartmentmanage.h \
    dialogordersearch.h \
    dialogplatingmanage.h \
    dialogorderproduceorout.h \
    goodsoutrecord.h \
    dialogoutgoods.h \
    dialoggoodsin.h \
    dialoggoodsoutrecordprint.h \
    formfinance.h \
    dialogprintouttable.h \
    formmatermanager.h \
    version.h

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
    dialoggoodsprint.ui \
    dialognewgoodstype.ui \
    formgoodstypemanage.ui \
    dialoggoodsprint.ui \
    dialogmaterialmanage.ui \
    dialogmodprice.ui \
    dialognewcustom.ui \
    dialognewgoods.ui \
    dialognewgoodstype.ui \
    dialognewmateriel.ui \
    dialogneworder.ui \
    dialognewsupplier.ui \
    dialognewunit.ui \
    dialognewuser.ui \
    dialogorderdetail.ui \
    dialogorderprint.ui \
    dialoguserprint.ui \
    formauthormanage.ui \
    formcenter.ui \
    formcustommanage.ui \
    formgoodstypemanage.ui \
    formsuppliermanage.ui \
    formunitmanage.ui \
    formwarehousemanage.ui \
    loadding.ui \
    login.ui \
    mainwindow.ui \
    ordermanager.ui \
    personmanager.ui \
    storemanager.ui \
    systemmanager.ui \
    dialognewdepartment.ui \
    formdepartmentmanage.ui \
    dialogordersearch.ui \
    dialogplatingmanage.ui \
    dialogorderproduceorout.ui \
    dialogoutgoods.ui \
    dialoggoodsin.ui \
    dialoggoodsoutrecordprint.ui \
    formfinance.ui \
    dialogprintouttable.ui \
    formmatermanager.ui

RESOURCES += \
    res.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Service/release/ -lService
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Service/debug/ -lService
else:unix: LIBS += -L$$OUT_PWD/../Service/ -lService

INCLUDEPATH += $$PWD/../Service
DEPENDPATH += $$PWD/../Service

DISTFILES += \
    app.rc

SUBDIRS += \
    ERP.pro
