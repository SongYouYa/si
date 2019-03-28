include(../../lib.pri)

mac {
CONFIG += staticlib
}
#else {
#LIBS += -lfilesystemshell \
#    -lsicommon \
#    -lsiutility

SOURCES += \
    stdafx.cpp \
    PLnkFile.cpp \
    encrypt/CheckHelper.cpp \
    encrypt/CommonCheck.cpp \
    encrypt/dmg_check.cpp \
    encrypt/doc_check.cpp \
    encrypt/PEncryptFile.cpp \
    encrypt/ppt_check.cpp \
    encrypt/xls_check.cpp

HEADERS += \
    stdafx.h \
    encrypt/all_check.h \
    encrypt/CheckHelper.h \
    encrypt/CommonCheck.h \
    ../../include/FileAnalyzer/PEncryptFile.h
