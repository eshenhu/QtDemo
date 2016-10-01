QT += core gui
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = piechartcustomization
TEMPLATE = app


SOURCES += main.cpp \
    pentool.cpp \
    brushtool.cpp \
    customslice.cpp \
    actionwidget.cpp \
    mainwindow.cpp \
    configtab.cpp

HEADERS += \
    pentool.h \
    brushtool.h \
    customslice.h \
    actionwidget.h \
    mainwindow.h \
    configtab.h \
    hwconfig_copy.h
