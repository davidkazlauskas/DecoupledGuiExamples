#-------------------------------------------------
#
# Project created by QtCreator 2015-04-26T09:12:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NthPrimeNumberGenerator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    domain.cpp

HEADERS  += mainwindow.h \
    mainwindow_interface.h \
    domain.h \
    messeagable.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$(TEMPLATIOUS_DIR)

CONFIG += c++11
