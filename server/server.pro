#-------------------------------------------------
#
# Project created by QtCreator 2017-01-17T16:21:42
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        servermainwindow.cpp \
    dbussession.cpp

HEADERS  += servermainwindow.h \
    dbussession.h

FORMS    += servermainwindow.ui
