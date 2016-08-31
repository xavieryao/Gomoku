#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T14:19:23
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gomoku
TEMPLATE = app


SOURCES += main.cpp\
        gomokuwidget.cpp \
    pawn.cpp \
    mainwindow.cpp \
    gomokuserver.cpp \
    gomokuclient.cpp \
    protocolserializer.cpp

HEADERS  += gomokuwidget.h \
    pawn.h \
    map.h \
    mainwindow.h \
    gomokuserver.h \
    gomokuclient.h \
    protocolserializer.h

RESOURCES += \
    res.qrc
