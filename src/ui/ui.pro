#-------------------------------------------------
#
# Project created by QtCreator 2012-11-21T11:12:53
#
#-------------------------------------------------

QT       -= gui

TARGET = ui
TEMPLATE = lib

DEFINES += UI_LIBRARY

SOURCES += IMMPui.cpp

HEADERS += IMMPui.h\
        ui_global.h \
    ../errors.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}