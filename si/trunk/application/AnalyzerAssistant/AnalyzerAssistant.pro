include(../../lib.pri)

mac {
CONFIG += staticlib
}

SOURCES += \
    stdafx.cpp \
    PAnalyzerAssistant.cpp \
    POsChecker.cpp \
    PAppAnalyzer.cpp \
    PCallbackFactory.cpp

HEADERS += \
    stdafx.h \
    POsChecker.h \
    ../../include/application/AnalyzerAssistant/PAnalyzerAssistant.h \
    PAppAnalyzer.h \
    PCallbackFactory.h
