#-------------------------------------------------
#
# Project created by QtCreator 2012-11-21T11:16:39
#
#-------------------------------------------------

QT       -= gui
QT += widgets

TARGET = logic
TEMPLATE = lib
CONFIG += staticlib

DEFINES += LOGIC_LIBRARY

SOURCES += manager.cpp \
    rules/gamemmp.cpp \
    rules/gameknights.cpp \
    logic.cpp \
    gamerules.cpp \
    rules/gamefight.cpp

HEADERS += manager.h\
        logic_global.h \
    rules/gamemmp.h \
    rules/gameknights.h \
    logic.h \
    gamerules.h \
    rules/gamefight.h \
    lerrors.h

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ui/release/ -lui
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ui/debug/ -lui
#else:unix: LIBS += -L$$OUT_PWD/../ui/ -lui

#INCLUDEPATH += $$PWD/../ui
#DEPENDPATH += $$PWD/../ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ui/release/ -lui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ui/debug/ -lui
else:unix:!macx: LIBS += -L$$OUT_PWD/../ui/ -lui

INCLUDEPATH += $$PWD/../ui
DEPENDPATH += $$PWD/../ui

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ui/release/ui.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ui/debug/ui.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../ui/libui.a
