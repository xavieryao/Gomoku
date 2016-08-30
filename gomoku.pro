#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T14:19:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gomoku
TEMPLATE = app


SOURCES += main.cpp\
        gomokuwidget.cpp \
    pawn.cpp

HEADERS  += gomokuwidget.h \
    pawn.h \
    map.h

RESOURCES += \
    res.qrc
