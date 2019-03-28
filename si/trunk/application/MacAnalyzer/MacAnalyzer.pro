include(../../lib.pri)

QT += core sql xml

mac {
CONFIG += staticlib
}
#else {
#LIBS += \
#    -lfilesystemshell \
#    -lbase \
#    -lsicommon \
#    -lboost_iostreams-mt -lboost_system-mt -lboost_filesystem-mt \
#    -lboost_regex-mt -lboost_program_options-mt \
#    -lplistcpp \
#    -lsqlite3 \
#    -liconv \
#    -lsiutility \
#    -lCommonAnalyzer \
#    -ltask \
#    -lTemplateFile \
#    -lFileAnalyzer \
#    -lDeviceEnumrator \
#    -lsidevice

HEADERS += \
    ../../include/application/MacAnalyzer/PMacAnalyzer.h \
    ../../include/application/MacAnalyzer/PMacCallbackFactory.h \
    SystemInfo/PAppListAnalyzer.h \
    SystemInfo/PAppListTaskHelper.h \
    SystemInfo/PComputerNameParser.h \
    SystemInfo/PSysBaseInfoAnalyzer.h \
    SystemInfo/PShareListAnalyzer.h \
    SystemInfo/PUseInfoAnalyzer.h \
    SystemInfo/PAutorunParser.h \
    SystemInfo/PAutorunStruct.h \
    SystemInfo/PLaunchParser.h \
    SIBrowser/PBrowserBookmark.h \
    SIBrowser/PBrowserInformation.h \
    SIBrowser/PChromeCacheParser.h \
    SIBrowser/PChromeCacheStruct.h \
    SIBrowser/PChromeParser.h \
    SIBrowser/PSafariCookieDef.h \
    SIBrowser/PSafariParser.h \
    stdafx.h \
    macim/PAdiumTaskHelper.h \
    macim/PAdiumAnalyzer.h \
    macim/PImessageAnalyzer.h \
    macim/PImessageStruct.h \
    macim/PImessageTaskHelper.h \
    macim/PSkypeAnalyzer.h \
    macim/PSkypeHelper.h \
    MacMailAnalyzer/PAppleMailAnalyzer.h \
    MacMailAnalyzer/PAppleMailHelper.h \
    MacMailAnalyzer/PMail.h \
    MacMailAnalyzer/PMailStruct.h \
    MacMailAnalyzer/PMailTaskHelper.h \
    MacMailAnalyzer/PMime.h \
    MacMailAnalyzer/PMimeChar.h \
    MacMailAnalyzer/PMimeCode.h \
    MacMailAnalyzer/PThunderBirdAnalyzer.h \
    MacMailAnalyzer/PThunderBirdHelper.h \
    UserActivity/PDailyOutAnalyzer.h \
    UserActivity/PLogsTaskHelper.h \
    UserActivity/PLogStruct.h \
    UserActivity/PSysLogAnallyzer.h \
    UserActivity/PAddressStruct.h \
    UserActivity/PAddressBookAnalyzer.h \
    UserActivity/PAddressTaskHelper.h \
    UserActivity/PRecentlyUsedItemHelper.h \
    UserActivity/PLogListAnalyzer.h \
    UserActivity/PEncryptFileList.h


SOURCES += \
    stdafx.cpp \
    PMacAnalyzer.cpp \
    PMacCallbackFactory.cpp \
    SystemInfo/PAppListAnalyzer.cpp \
    SystemInfo/PAppListTaskHelper.cpp \
#    SystemInfo/PComputerNameParser.cpp \
    SystemInfo/PSysBaseInfoAnalyzer.cpp \
    SystemInfo/PShareListAnalyzer.cpp \
    SystemInfo/PUseInfoAnalyzer.cpp \
    SystemInfo/PAutorunParser.cpp \
    SystemInfo/PLaunchParser.cpp \
    SIBrowser/PBrowserBookmark.cpp \
    SIBrowser/PChromeCacheParser.cpp \
    SIBrowser/PChromeParser.cpp \
    SIBrowser/PSafariParser.cpp \
    macim/PAdiumAnalyzer.cpp \
    macim/PAdiumTaskHelper.cpp \
    macim/PImessageAnalyzer.cpp \
    macim/PImessageTaskHelper.cpp \
    macim/PSkypeAnalyzer.cpp \
    macim/PSkypeHelper.cpp \
    MacMailAnalyzer/PAppleMailAnalyzer.cpp \
    MacMailAnalyzer/PAppleMailHelper.cpp \
    MacMailAnalyzer/PMail.cpp \
    MacMailAnalyzer/PMime.cpp \
    MacMailAnalyzer/PMimeChar.cpp \
    MacMailAnalyzer/PMimeCode.cpp \
    MacMailAnalyzer/PMimeType.cpp \
    MacMailAnalyzer/PThunderBirdAnalyzer.cpp \
    MacMailAnalyzer/PThunderBirdHelper.cpp \
    MacMailAnalyzer/PMailTaskHelper.cpp \
    UserActivity/PDailyOutAnalyzer.cpp \
    UserActivity/PLogsTaskHelper.cpp \
    UserActivity/PSysLogAnallyzer.cpp \
    UserActivity/PAddressStruct.cpp \
    UserActivity/PAddressBookAnalyzer.cpp \
    UserActivity/PAddressTaskHelper.cpp \
    UserActivity/PRecentlyUsedItemHelper.cpp \
    UserActivity/PLogListAnalyzer.cpp \
    UserActivity/PEncryptFileList.cpp


