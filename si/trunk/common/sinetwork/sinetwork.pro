include(../../lib.pri)

mac {
CONFIG += staticlib
}
#else {
#LIBS += -lboost_regex-mt
#    -lsicommon

SOURCES += \
    stdafx.cpp \
    PNetwork.cpp \
    PShareFolder.cpp

HEADERS += \
    stdafx.h \
    ../../include/sinetwork/PNetwork.h \
    ../../include/sinetwork/PShareFolder.h
