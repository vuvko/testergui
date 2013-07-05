#-------------------------------------------------
#
# Project created by QtCreator 2012-11-21T11:38:19
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = console
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    consoleui.cpp \
    arg_parser.cpp \
    application.cpp

HEADERS += \
    consoleui.h \
    arg_parser.h \
    application.h

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ui/release/ -lui
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ui/debug/ -lui
#else:unix: LIBS += -L$$OUT_PWD/../ui/ -lui

#INCLUDEPATH += $$PWD/../ui
#DEPENDPATH += $$PWD/../ui

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logic/release/ -llogic
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logic/debug/ -llogic
#else:unix: LIBS += -L$$OUT_PWD/../logic/ -llogic

#INCLUDEPATH += $$PWD/../logic
#DEPENDPATH += $$PWD/../logic

RESOURCES += \
    res.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logic/release/ -llogic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logic/debug/ -llogic
else:unix: LIBS += -L$$OUT_PWD/../logic/ -llogic

INCLUDEPATH += $$PWD/../logic
DEPENDPATH += $$PWD/../logic

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logic/release/logic.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logic/debug/logic.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../logic/liblogic.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ui/release/ -lui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ui/debug/ -lui
else:unix:!macx: LIBS += -L$$OUT_PWD/../ui/ -lui

INCLUDEPATH += $$PWD/../ui
DEPENDPATH += $$PWD/../ui

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ui/release/ui.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ui/debug/ui.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../ui/libui.a
