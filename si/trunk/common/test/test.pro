include(../../lib.pri)

QT += core gui widgets

TEMPLATE = app
TARGET = test

LIBS += \
    -lsinetwork


HEADERS += \
    stdafx.h

SOURCES += \
    stdafx.cpp \
    test.cpp
