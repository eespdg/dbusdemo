#-------------------------------------------------
#
# Project created by QtCreator 2017-01-18T11:15:32
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app

DBUS_INTERFACES += ../common/car.xml


SOURCES += main.cpp\
        clientmainwindow.cpp

HEADERS  += clientmainwindow.h

FORMS    += clientmainwindow.ui

SRCFILE = $$PWD/../lib/dbus-1.dll
DDIR = $$DESTDIR
# Replace slashes in paths with backslashes for Windows
win32:SRCFILE ~= s,/,\\,g
win32:DDIR ~= s,/,\\,g
win32:QMAKE_POST_LINK += $$QMAKE_COPY .
