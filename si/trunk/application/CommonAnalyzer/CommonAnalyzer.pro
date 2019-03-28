include(../../lib.pri)

CONFIG += staticlib

SOURCES += \
    stdafx.cpp \
    PDeviceInfoParser.cpp \
    PCommonCallbackFactory.cpp

HEADERS += \
    stdafx.h \
    ../../include/application/CommonAnalyzer/PAppParam.h \
    ../../include/application/CommonAnalyzer/PDeviceInfoParser.h \
    ../../include/application/CommonAnalyzer/PCommonCallbackFactory.h

