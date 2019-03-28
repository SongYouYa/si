include(../../lib.pri)

QT += core xml

mac {
CONFIG += staticlib
}
#else {
#LIBS += -lfilesystemshell \
#    -lboost_system-mt \
#    -lsicommon \
#    -lsiutility
#}

SOURCES += \
    stdafx.cpp \
    AppRoot.cpp \
    AppTarget.cpp \
    PathRegex.cpp \
    PTemplateFile.cpp \
    TemplateApp.cpp

HEADERS += \
    stdafx.h \
    AppRoot.h \
    AppTarget.h \
    PathRegex.h \
    TemplateApp.h \
    ../../include/TemplateFile/PTemplateFile.h
