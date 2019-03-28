include(../../lib.pri)

mac {
CONFIG += staticlib
}

SOURCES += \
    stdafx.cpp \
    PWindowsAnalyzer.cpp \
    PWinCallbackFactory.cpp \
    basic/PApplistParser.cpp \
    basic/POsBaseParser.cpp \
    material/PFsRegFile.cpp \
    material/PRegParserUtils.cpp

HEADERS += \
    stdafx.h \
    basic/PApplistParser.h \
    basic/POsBaseParser.h \
    material/PFsRegFile.h \
    material/PRegParserUtils.h \
    ../../include/application/WindowsAnalyzer/PWindowsAnalyzer.h \
    ../../include/application/WindowsAnalyzer/PWinCallbackFactory.h
