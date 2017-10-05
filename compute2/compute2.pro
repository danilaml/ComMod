#-------------------------------------------------
#
# Project created by QtCreator 2017-10-04T15:28:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = compute2
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        MainWindow.cpp \
    AntGameField.cpp

HEADERS += \
        MainWindow.h \
    AntGameField.h

FORMS += \
        MainWindow.ui
