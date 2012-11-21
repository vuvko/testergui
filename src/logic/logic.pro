#-------------------------------------------------
#
# Project created by QtCreator 2012-11-21T11:16:39
#
#-------------------------------------------------

QT       -= gui

TARGET = logic
TEMPLATE = lib

DEFINES += LOGIC_LIBRARY

SOURCES += manager.cpp \
    rules/gamemmp.cpp \
    rules/gameknights.cpp \
    logic.cpp \
    gamerules.cpp

HEADERS += manager.h\
        logic_global.h \
    rules/gamemmp.h \
    rules/gameknights.h \
    logic.h \
    gamerules.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ui/release/ -lui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ui/debug/ -lui
else:unix: LIBS += -L$$OUT_PWD/../ui/ -lui

INCLUDEPATH += $$PWD/../ui
DEPENDPATH += $$PWD/../ui
