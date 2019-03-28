include(../../lib.pri)

CONFIG += staticlib

SOURCES += \
    stdafx.cpp \
    PAffFile.cpp

HEADERS += \
    stdafx.h \
    ../../include/sifile/PAffFile.h \
    ../../include/sifile/PMemoryReader.h

mac {
    SOURCES += \
    mac/PMemoryReader.cpp
}
else {
    SOURCES += \
    linux/PMemoryReaderLinux.cpp
}
