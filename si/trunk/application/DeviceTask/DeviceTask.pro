include(../../lib.pri)

QT += core

mac {
CONFIG += staticlib
}
#else {
#LIBS += -lsicommon \
#    -lbase \
#    -ltask \
#    -lDeviceEnumrator \
#    -lsidevice \
#    -lboost_filesystem-mt -lboost_system-mt \
#    -llibdevtask \
#    -lScreenRecorderLoader \
#    -lsiio \
#    -lplog \
#    -lfilesystemshell \
#    -lsiutility


SOURCES += \
    stdafx.cpp \
    PDeviceCloneTask.cpp \
    PDeviceEraseTask.cpp \
    PDeviceImageTask.cpp \
    stdafx.cpp \
    material/PSplitDdFile.cpp \
    PScreenRecordTask.cpp \
    PExportFilesTask.cpp \
    fileoper/PFileHandleHelper.cpp \
    fileoper/PLogger.cpp \
    cache/PImageTaskCacheImpl.cpp \
    cache/PCloneTaskCacheImpl.cpp \
    cache/PCloneTaskCache.cpp \
    cache/PImageTaskCache.cpp \
    PSiDeviceTaskCommon.cpp

HEADERS += \
    stdafx.h \
    ../../include/application/DeviceTask/globals.h \
    ../../include/application/DeviceTask/PDeviceCloneTask.h \
    ../../include/application/DeviceTask/PDeviceEraseTask.h \
    ../../include/application/DeviceTask/PDeviceImageTask.h \
    ../../include/application/DeviceTask/PDeviceTaskParam.h \
    material/PSplitDdFile.h \
    ../../include/application/DeviceTask/PExportFilesTask.h \
    ../../include/application/DeviceTask/PScreenRecordTask.h \
    ../../include/application/DeviceTask/cache/PImageTaskCache.h \
    cache/PImageTaskCacheImpl.h \
    cache/PCloneTaskCacheImpl.h \
    PFilePackageTask.h \
    PSiDeviceTaskCommon.h


mac {
HEADERS += \
    PFilePackageTask.h
SOURCES += \
    PFilePackageTask.cpp
}
