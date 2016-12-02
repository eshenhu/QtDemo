QT += core gui
QT += charts
QT += serialport widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = piechartcustomization
TEMPLATE = app
CONFIG += c++11

win32:RC_ICONS += favicon.ico

SOURCES += main.cpp \
    actionwidget.cpp \
    mainwindow.cpp \
    ui/configtab.cpp \
    ui/testinfoconfig.cpp \
    ui/optionsframe.cpp \
    comm/qmodbusclient.cpp \
    comm/qmodbusdataunit.cpp \
    comm/qmodbusdevice.cpp \
    comm/qmodbusdeviceidentification.cpp \
    comm/qmodbuspdu.cpp \
    comm/qmodbusreply.cpp \
    comm/qmodbusrtuserialmaster.cpp \
    driver/automationmodeldriverclz.cpp \
    driver/basedmodeldriverclz.cpp \
    ui/testtab.cpp \
    cfg/cfgreshandler.cpp \
    driver/measdataformat.cpp \
    util/fileassist.cpp \
    util/filefactory.cpp \
    util/filemeas.cpp \
    cfg/cfgjsonprimaryelement.cpp \
    ui/abstractspinboxattr.cpp \
    ui/qextcheckbox.cpp \
    ui/compqchartwidget.cpp \
    ui/qrtlineseries.cpp \
    util/qserialporthelper.cpp \
    util/utildatarecordingclz.cpp \
    cfg/cfgjsonrecelement.cpp \
    cfg/datajsonrecelement.cpp \
    chartsview/chartsviewheaders.cpp \
    ui/qcxtchart.cpp \
    chartsview/chartviewerwin.cpp \
    chartsview/multichartwidget.cpp \
    chartsview/customtablemodel.cpp

HEADERS += \
    actionwidget.h \
    mainwindow.h \
    ui/configtab.h \
    ui/testinfoconfig.h \
    ui/optionsframe.h \
    comm/qmodbus_symbols_p.h \
    comm/qmodbusadu_p.h \
    comm/qmodbusclient.h \
    comm/qmodbusclient_p.h \
    comm/qmodbuscommevent_p.h \
    comm/qmodbusdataunit.h \
    comm/qmodbusdevice.h \
    comm/qmodbusdevice_p.h \
    comm/qmodbusdeviceidentification.h \
    comm/qmodbuspdu.h \
    comm/qmodbusreply.h \
    comm/qmodbusrtuserialmaster.h \
    comm/qmodbusrtuserialmaster_p.h \
    comm/qserialbusglobal.h \
    driver/automationmodeldriverclz.h \
    driver/basedmodeldriverclz.h \
    ui/testtab.h \
    cfg/cfgreshandler.h \
    cfg/cfgreshandlerinf.h \
    driver/modelpoctype.h \
    driver/measdataformat.h \
    util/fileassist.h \
    util/filefactory.h \
    util/filemeas.h \
    util/csv.h \
    driver/signaloverline.h \
    cfg/cfgjsonprimaryelement.h \
    ui/basedspinboxattrinf.h \
    ui/abstractspinboxattr.h \
    ui/functions.h \
    ui/qextcheckbox.h \
    ui/compqchartwidget.h \
    ui/qrtlineseries.h \
    util/qserialporthelper.h \
    util/dserialportsetting.h \
    util/utildatarecordingclz.h \
    cfg/cfgjsonrecelement.h \
    cfg/datajsonrecelement.h \
    ui/uiheader.h \
    chartsview/chartsviewheaders.h \
    ui/qcxtchart.h \
    unireslocation.h \
    chartsview/chartviewerwin.h \
    chartsview/multichartwidget.h \
    chartsview/customtablemodel.h

FORMS += \
    ui/testinfoconfig.ui \
    ui/optionsframe.ui \
    chartsview/chartviewerwin.ui

SUBDIRS += \
    comm/doc/snippets/snippets.pro \
    comm/serialbus.pro

DISTFILES +=

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/release/ -lqtcsv.1.0.0
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/debug/ -lqtcsv.1.0.0
#else:unix: LIBS += -L$$PWD/../../../../../../usr/local/lib/ -lqtcsv.1.0.0

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/release/
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/debug/
else:unix: LIBS += -L$$PWD/../../../../../../usr/local/lib/

#INCLUDEPATH += $$PWD/../../../../../../usr/local/include/qtcsv
#DEPENDPATH += $$PWD/../../../../../../usr/local/include/qtcsv

RESOURCES += \
    resource/productversion.qrc
