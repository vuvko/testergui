#-------------------------------------------------
#
# Project created by QtCreator 2012-11-21T11:10:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app


SOURCES += main.cpp \
        window.cpp \
    IMMPGuiImpl.cpp \
    gamechooser.cpp \
    FieldWidget.cpp

HEADERS  += window.h \
    QFieldObject.h \
    IMMPGuiImpl.h \
    gamechooser.h \
    FieldWidget.h

FORMS    += window.ui \
    gamechooser.ui

RESOURCES += \
    res.qrc

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ui/release/ -lui
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ui/debug/ -lui
#else:unix:!macx: LIBS += -L$$OUT_PWD/../ui/ -lui

#INCLUDEPATH += $$PWD/../ui
#DEPENDPATH += $$PWD/../ui

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logic/release/ -llogic
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logic/debug/ -llogic
#else:unix:!macx: LIBS += -L$$OUT_PWD/../logic/ -llogic

#INCLUDEPATH += $$PWD/../logic
#DEPENDPATH += $$PWD/../logic

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logic/release/ -llogic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logic/debug/ -llogic
else:unix:!macx: LIBS += -L$$OUT_PWD/../logic/ -llogic

INCLUDEPATH += $$PWD/../logic
DEPENDPATH += $$PWD/../logic

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logic/release/logic.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logic/debug/logic.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../logic/liblogic.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ui/release/ -lui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ui/debug/ -lui
else:unix:!macx: LIBS += -L$$OUT_PWD/../ui/ -lui

INCLUDEPATH += $$PWD/../ui
DEPENDPATH += $$PWD/../ui

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ui/release/ui.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ui/debug/ui.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../ui/libui.a
