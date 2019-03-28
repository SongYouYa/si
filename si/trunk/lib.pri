
mac {
gccdir = clang11
libdir = mac/x64
QMAKE_CC = clang
QMAKE_CXX = clang++
} else {
gccdir = gcc7
libdir = linux/x64
QMAKE_CC = gcc
QMAKE_CXX = g++
}

QT -= core gui
TEMPLATE = lib
CONFIG += debug_and_release warn_on plugin
CONFIG += C++11


CONFIG(debug,debug|release):{
    DESTDIR = $$_PRO_FILE_PWD_/../../libs-build/debug/bin
    LIBS += -L$$_PRO_FILE_PWD_/../../libs-build/debug/bin
    LIBS += -L$$_PRO_FILE_PWD_/../../lib/$${libdir}/$${gccdir}/debug
    DEFINES += _DEBUG
}

CONFIG(release,debug|release):{
    DESTDIR = $$_PRO_FILE_PWD_/../../libs-build/release/bin
    LIBS += -L$$_PRO_FILE_PWD_/../../libs-build/release/bin
    LIBS += -L$$_PRO_FILE_PWD_/../../lib/$${libdir}/$${gccdir}/release
}

LIBS += -L$$_PRO_FILE_PWD_/../../lib/$${libdir}/$${gccdir}
mac {
} else {
LIBS += -L$$_PRO_FILE_PWD_/../../lib/$${libdir}/$${gccdir}/qt
}

INCLUDEPATH += $$_PRO_FILE_PWD_/../../include

CONFIG += precompile_header
PRECOMPILED_HEADER=stdafx.h

include(warning_disable.pri)
