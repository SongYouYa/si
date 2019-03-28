include(../../lib.pri)

QT += core sql

HEADERS += \
    stdafx.h \
    SystemInfo/include/systeminfo/DeviceInfoAnalyzer.h \
    SystemInfo/include/systeminfo/PartitionAnalyzer.h \
    SystemInfo/include/systeminfo/ShareListAnalyzer.h \
    MacIMAnalyzer/include/macim/adium/AdiumTaskHelper.h \
    MacIMAnalyzer/include/macim/adium/PAdiumAnalyzer.h \
    MacIMAnalyzer/include/macim/adium/PAdiumStruct.h \
    MacIMAnalyzer/include/macim/imessage/IMessageStruct.h \
    MacIMAnalyzer/include/macim/imessage/IMessageTaskHelper.h \
    MacIMAnalyzer/include/macim/imessage/PIMessageAnalyzer.h \
    MacIMAnalyzer/include/macim/skype/SkypeAnalyzer.h \
    MacIMAnalyzer/include/macim/skype/SkypeHelper.h \
    MacIMAnalyzer/include/macim/IMStruct.h \
    SIBrowser/include/SIBrowser/BrowserBookmark.h \
    SIBrowser/include/SIBrowser/BrowserInformation.h \
    SIBrowser/include/SIBrowser/ChromeCacheParser.h \
    SIBrowser/include/SIBrowser/ChromeCacheStruct.h \
    SIBrowser/include/SIBrowser/ChromeParser.h \
    SIBrowser/include/SIBrowser/SafariCookieDef.h \
    SIBrowser/include/SIBrowser/SafariParser.h \
    SystemInfo/include/systeminfo/GetComputerName.h \
    SystemInfo/include/systeminfo/UseInfoAnalyzer.h \
    SystemInfo/include/systeminfo/autorun/AutorunParser.h \
    SystemInfo/include/systeminfo/autorun/AutorunStruct.h \
    SystemInfo/include/systeminfo/autorun/LaunchParser.h \
    SystemInfo/include/systeminfo/applist/AppListAnalyzer.h \
    SystemInfo/include/systeminfo/applist/AppListTaskHelper.h \
    ../../include/application/MacBasic/globals.h \
    ../../include/application/MacBasic/PMacBasicTask.h \
    SystemInfo/include/systeminfo/SysBaseInfoAnalyzer.h \
#    MacMailAnalyzer/include/macmail/applemail/AppleMailAnalyzer.h \
#    MacMailAnalyzer/include/macmail/applemail/AppleMailHelper.h \
#    MacMailAnalyzer/include/macmail/thunderbird/ThunderBirdAnalyzer.h \
#    MacMailAnalyzer/include/macmail/thunderbird/ThunderBirdHelper.h \
#    MacMailAnalyzer/include/macmail/MailTaskHelper.h

SOURCES += \
    SystemInfo/src/DeviceInfoAnalyzer.cpp \
    SystemInfo/src/PartitionAnalyzer.cpp \
    SystemInfo/src/ShareListAnalyzer.cpp \
    MacIMAnalyzer/src/adium/AdiumTaskHelper.cpp \
    MacIMAnalyzer/src/adium/PAdiumAnalyzer.cpp \
    MacIMAnalyzer/src/imessage/IMessageTaskHelper.cpp \
    MacIMAnalyzer/src/imessage/PIMessageAnalyzer.cpp \
    MacIMAnalyzer/src/skype/SkypeAnalyzer.cpp \
    MacIMAnalyzer/src/skype/SkypeHelper.cpp \
    SIBrowser/src/BrowserBookmark.cpp \
    SIBrowser/src/ChromeCacheParser.cpp \
    SIBrowser/src/ChromeParser.cpp \
    SIBrowser/src/SafariParser.cpp \
    SystemInfo/src/GetComputerName.cpp \
    SystemInfo/src/UseInfoAnalyzer.cpp \
    SystemInfo/src/autorun/AutorunParser.cpp \
    SystemInfo/src/autorun/LaunchParser.cpp \
    PMacBasicTask.cpp \
    SystemInfo/src/applist/AppListAnalyzer.cpp \
    SystemInfo/src/applist/AppListTaskHelper.cpp \
    SystemInfo/src/SysBaseInfoAnalyzer.cpp \
#    MacMailAnalyzer/src/applemail/AppleMailAnalyzer.cpp \
#    MacMailAnalyzer/src/applemail/AppleMailHelper.cpp \
#    MacMailAnalyzer/src/thunderbird/ThunderBirdAnalyzer.cpp \
#    MacMailAnalyzer/src/thunderbird/ThunderBirdHelper.cpp \
#    MacMailAnalyzer/src/MailTaskHelper.cpp
