#-------------------------------------------------
#
# Project created by QtCreator 2017-01-18T11:15:32
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app

DBUS_INTERFACES += \
    ../common/dbusmonitor.xml \
    ../common/vehicle.xml


SOURCES += main.cpp\
        clientmainwindow.cpp \
    dbusclientconnection.cpp

HEADERS  += clientmainwindow.h \
    dbusclientconnection.h \
    dbusclientconnection_p.h

FORMS    += clientmainwindow.ui

SRCFILE = $$PWD/../lib/dbus-1.dll
DDIR = $$DESTDIR
# Replace slashes in paths with backslashes for Windows
win32:SRCFILE ~= s,/,\\,g
win32:DDIR ~= s,/,\\,g
win32:QMAKE_POST_LINK += $$QMAKE_COPY $$SRCFILE .
