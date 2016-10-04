QT += core gui
QT += charts
QT += serialport widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = piechartcustomization
TEMPLATE = app


SOURCES += main.cpp \
    pentool.cpp \
    brushtool.cpp \
    customslice.cpp \
    actionwidget.cpp \
    mainwindow.cpp \
    ui/settingsdialog.cpp \
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
    driver/basedmodeldriverclz.cpp

HEADERS += \
    pentool.h \
    brushtool.h \
    customslice.h \
    actionwidget.h \
    mainwindow.h \
    ui/settingsdialog.h \
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
    driver/basedmodeldriverclz.h

FORMS += \
    ui/settingsdialog.ui \
    ui/testinfoconfig.ui \
    ui/optionsframe.ui

SUBDIRS += \
    comm/doc/snippets/snippets.pro \
    comm/serialbus.pro

DISTFILES += \
