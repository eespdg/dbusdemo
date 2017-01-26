#-------------------------------------------------
#
# Project created by QtCreator 2017-01-17T16:21:42
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app

INCLUDEPATH += ../common

DBUS_ADAPTORS += \
    ../common/dbusmonitor.xml \
    ../common/vehicle.xml

SOURCES += \
    main.cpp\
    servermainwindow.cpp \
    vehicle.cpp \
    dbusmonitor.cpp

HEADERS  += \
    servermainwindow.h \
    vehicle.h \
    dbusmonitor.h

FORMS    += servermainwindow.ui

SRCFILE = $$PWD/../lib/dbus-1.dll
DDIR = $$DESTDIR
# Replace slashes in paths with backslashes for Windows
win32:SRCFILE ~= s,/,\\,g
win32:DDIR ~= s,/,\\,g
win32:QMAKE_POST_LINK += $$QMAKE_COPY $$SRCFILE .
