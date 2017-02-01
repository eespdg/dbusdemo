#-------------------------------------------------
#
# Project created by QtCreator 2017-01-30T11:11:31
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dbustest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../common ../dbus

DBUS_ADAPTORS += \
    ../common/vehicle.xml

DBUS_PROXIES += \
    ../common/vehicle.xml

SOURCES += main.cpp\
        dbustest.cpp \
    ../dbus/dbusclient.cpp \
    ../dbus/dbusobjectwatcher.cpp \
    ../dbus/dbusservicemonitor.cpp \
    vehicle.cpp

HEADERS  += dbustest.h \
    ../dbus/dbusclient.h \
    ../dbus/dbusclient_p.h \
    ../dbus/dbusobjectwatcher.h \
    ../dbus/dbusobjectwatcher_p.h \
    ../dbus/dbusservicemonitor.h \
    ../dbus/dbusservicemonitor_p.h \
    vehicle.h

FORMS    += dbustest.ui

SRCFILE = $$PWD/../lib/dbus-1.dll
DDIR = $$DESTDIR
# Replace slashes in paths with backslashes for Windows
win32:SRCFILE ~= s,/,\\,g
win32:DDIR ~= s,/,\\,g
win32:QMAKE_POST_LINK += $$QMAKE_COPY $$SRCFILE .
