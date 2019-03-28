include(../../lib.pri)

mac {
CONFIG += staticlib
}
#else {
#LIBS += -lfilesystemshell \
#    -lsicommon \
#    -ltask \
#    -lbase \
#    -lsiio \
#    -lboost_filesystem-mt -lboost_system-mt \
#    -lsidevice \
#    -liconv
#}

HEADERS += \
    ../../include/siutility/globals.h \
    ../../include/siutility/PFileSiUtils.h \
    ../../include/siutility/PDirHelper.h \
    ../../include/siutility/MacCommonHelper.h \
    ../../include/siutility/PRecordHandler.h \
    ../../include/siutility/PDevicePath.h \
    ../../include/siutility/FileSearch.h \
    ../../include/siutility/PMacFileExport.h \
    stdafx.h \
    ../../include/siutility/PFileExport.h \
    ../../include/siutility/PCodeConverter.h \
    ../../include/siutility/PFileOperate.h

SOURCES += \
    PFileSiUtils.cpp \
    stdafx.cpp \
    PDirHelper.cpp \
    MacCommonHelper.cpp \
    FileSearch.cpp \
    PRecordHandler.cpp \
    PDevicePath.cpp \
    PMacFileExport.cpp \
    PFileExport.cpp \
    PCodeConverter.cpp \
    FileOperate.cpp
