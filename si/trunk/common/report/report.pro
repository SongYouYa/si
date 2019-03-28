include(../../lib.pri)

mac {
CONFIG += staticlib
INCLUDEPATH += $$_PRO_FILE_PWD_/../../include/ctemplate/mac
}
else {
INCLUDEPATH += $$_PRO_FILE_PWD_/../../include/ctemplate/linux
}

#LIBS += \
#    -lctemplate \
#    -lfilesystemshell \
#    -lsicommon \
#    -lbase \
#    -ljsoncpp \
#    -lboost_filesystem-mt -lboost_system-mt \
#    -lsiutility

DEFINES += CTEMPLATE_DLL_DECL

SOURCES += \
    stdafx.cpp \
    PLanguageSource.cpp \
    PRecordReport.cpp \
    PReportConfig.cpp \
    PReportPath.cpp \
    PReportUtility.cpp \
    handler/PReportHandler.cpp \
    material/PCell.cpp \
    material/PPage.cpp \
    material/PPageJumper.cpp \
    material/PPageSet.cpp \
    material/PRow.cpp \
    material/PTable.cpp \
    material/PTree.cpp \
    PRecordReport2.cpp \
    material/PContent2.cpp \
    material/PTable2.cpp \
    material/PTree2.cpp \
    material/PTableBase.cpp

HEADERS += \
    stdafx.h \
    PLanguageSource.h \
    PReportPath.h \
    PUtil.h \
    handler/PReportHandler.h \
    material/PCell.h \
    material/PCssDefine.h \
    material/PPage.h \
    material/PPageJumper.h \
    material/PPageSet.h \
    material/PRow.h \
    material/PTable.h \
    material/PTree.h \
    ../../include/report/globals.h \
    ../../include/report/PRecordReport.h \
    ../../include/report/PReportConfig.h \
    ../../include/report/PReportUtility.h \
    material/PContent2.h \
    material/PTable2.h \
    material/PTree2.h \
    material/PTableBase.h \
    ../../include/report/PRecordReport2.h
