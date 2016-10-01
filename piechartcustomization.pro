QT += core gui
QT += charts
QT += serialbus serialport widgets

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
    ui/testinfoconfig.cpp

HEADERS += \
    pentool.h \
    brushtool.h \
    customslice.h \
    actionwidget.h \
    mainwindow.h \
    ui/settingsdialog.h \
    ui/configtab.h \
    ui/testinfoconfig.h

FORMS += \
    ui/settingsdialog.ui \
    ui/testinfoconfig.ui
