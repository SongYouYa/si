include(../../lib.pri)

mac {
CONFIG += staticlib
}
#else {
#LIBS += -lbase \
#    -lboost_system-mt
#}

SOURCES += \
    FuncTypedef.cpp \
    PFileFilter.cpp \
    FsDllLoader.cpp \
    stdafx.cpp \
    FileSystemShell.cpp \
    FsDllLoaderLinux.cpp \
    PFileProperty.cpp \
    PFsStatus.cpp

HEADERS += \
    FuncTypedef.h \
    PFileFilter.h \
    FsDllLoader.h \
    targetver.h \
    stdafx.h \
    ../../include/filesystemshell/FileSystemShell.h \
    ../../include/filesystemshell/globals.h \
    ../../include/filesystemshell/PInterfaceStruct.h \
    ../../include/filesystemshell/PFileProperty.h \
    ../../include/filesystemshell/PFsStatus.h
