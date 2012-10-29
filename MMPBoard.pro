QT = core gui

RC_FILE = app.rc

INCLUDEPATH = . src src/gui src/logic

TARGET = MMP
TEMPLATE = app

HEADERS += src/window.h \
           src/gui/IMMPGui.h \
           src/gui/IMMPGuiImpl.h \
           src/gui/FieldWidget.h \
           src/logic/manager.h \
    src/logic/errors.h

SOURCES += src/main.cpp \
           src/window.cpp \
           src/gui/IMMPGui.cpp \
           src/gui/IMMPGuiImpl.cpp \
           src/gui/FieldWidget.cpp \
           src/logic/manager.cpp

RESOURCES += res.qrc

OTHER_FILES += \
    README \
    COPYING

FORMS +=


















































