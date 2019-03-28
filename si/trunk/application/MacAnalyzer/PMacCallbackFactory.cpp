#include "stdafx.h"
#include "application/MacAnalyzer/PMacCallbackFactory.h"
#include "SystemInfo/PAppListTaskHelper.h"
#include "SystemInfo/PSysBaseInfoAnalyzer.h"
#include "SystemInfo/PShareListAnalyzer.h"
#include "SystemInfo/PUseInfoAnalyzer.h"
#include "SystemInfo/PAutorunParser.h"
#include "SIBrowser/PSafariParser.h"
#include "SIBrowser/PChromeParser.h"
#include "macim/PAdiumTaskHelper.h"
#include "macim/PImessageTaskHelper.h"
#include "macim/PSkypeHelper.h"
#include "MacMailAnalyzer/PMailTaskHelper.h"
#include "MacMailAnalyzer/PAppleMailHelper.h"
#include "MacMailAnalyzer/PAppleMailHelper.h"
#include "MacMailAnalyzer/PThunderBirdHelper.h"
#include "UserActivity/PAddressTaskHelper.h"
#include "UserActivity/PLogsTaskHelper.h"
#include "UserActivity/PRecentlyUsedItemHelper.h"
#include "UserActivity/PLogListAnalyzer.h"
#include "UserActivity/PEncryptFileList.h"

using namespace macbasic;

namespace app
{
AppCallback CPMacCallbackFactory::getAppCallback(PSIHANDLE volume, CPRecord* folderRecord,
                                                 CPTaskStatusCtrl* statusCtrl)
{
    std::wstring appName = folderRecord->getType()->name();
    if(appName == L"table.mac.systembaseinfo"){
        CPSysBaseInfoAnalyzer baseInfoAnalyzer;
        return std::bind(&CPSysBaseInfoAnalyzer::analyze, baseInfoAnalyzer, folderRecord, true);
    }
    if(appName == L"table.mac.applist_sys"){
        return std::bind(CPAppListTaskHelper::analyze,folderRecord);
    }
    if(appName == L"table.mac.sharelistinfo"){
        return std::bind(CPShareListAnalyzer::analyze,folderRecord);
    }
    if(appName == L"table.mac.userinfo"){
        CPUseInfoAnalyzer infoAnalyzer;
        return std::bind(&CPUseInfoAnalyzer::GetUseInfo,infoAnalyzer,folderRecord);
    }
    if(appName == L"table.mac.autorun"){
        CPAutorunParser autoRunParser;
        return std::bind(&CPAutorunParser::GetAutorunInfo,autoRunParser,folderRecord);
    }
    //safari
    if(appName == L"table.mac.safarihistory"){
        CPSafariParser safariParser;
        return boost::bind(&CPSafariParser::GetHistory, safariParser,folderRecord);
    }
    if(appName == L"table.mac.safaridownload"){
        CPSafariParser safariParser;
        return boost::bind(&CPSafariParser::GetDownload, safariParser,folderRecord);
    }
    if(appName == L"table.mac.safaricache"){
        CPSafariParser safariParser;
        return boost::bind(&CPSafariParser::GetCache, safariParser,folderRecord);
    }
    if(appName == L"table.mac.safaribookmark"){
        CPSafariParser safariParser;
        return boost::bind(&CPSafariParser::GetBookmark, safariParser,folderRecord);
    }
    if(appName == L"table.mac.safaricookie"){
        CPSafariParser safariParser;
        return boost::bind(&CPSafariParser::GetCookie, safariParser,folderRecord);
    }
    //chrome
    if(appName == L"table.mac.chromehistory"){
        CPChromeParser chromeParserGistory;
        return boost::bind(&CPChromeParser::GetHistory, chromeParserGistory,folderRecord);
    }
    if(appName == L"table.mac.chromedownload"){
        CPChromeParser chromeParserDownload;
        return boost::bind(&CPChromeParser::GetDownload, chromeParserDownload, folderRecord);
    }
    if(appName == L"table.mac.chromecache"){
        CPChromeParser chromeParseCache;
        return boost::bind(&CPChromeParser::GetCache, chromeParseCache,folderRecord);
    }
    if(appName == L"table.mac.chromebookmark"){
        CPChromeParser chromeParserBookMark;
        return boost::bind(&CPChromeParser::GetBookmark, chromeParserBookMark,folderRecord);
    }
    if(appName == L"table.mac.chromecookie"){
        CPChromeParser chromeParseCookie;
        return boost::bind(&CPChromeParser::GetCookie, chromeParseCookie, folderRecord);
    }
    if(appName == L"table.mac.chromeform"){
        CPChromeParser chromeParserForm;
        return boost::bind(&CPChromeParser::GetForm, chromeParserForm,folderRecord);
    }
    //adium
    if(appName == L"table.mac.adiumaccount"){
        return boost::bind(&CPAdiumTaskHelper::GetAccountInfo, folderRecord);
    }
    if(appName == L"table.mac.adiumcontact"){
        return boost::bind(&CPAdiumTaskHelper::GetContactInfo,folderRecord);
    }
    if(appName == L"table.mac.adiummsg"){
        return boost::bind(&CPAdiumTaskHelper::GetMsgInfo,folderRecord);
    }
    //IMESSAGAE
    if(appName == L"table.mac.imessageaccount"){
        return boost::bind(&CPImessageTaskHelper::getIMessageAccountRecords,folderRecord);
    }
    if(appName == L"table.mac.imessagemsg"){
        return boost::bind(&CPImessageTaskHelper::getIMessageMsgRecords,folderRecord);
    }
    //skype
    if(appName == L"table.mac.skypeaccount"){
        return boost::bind(&CPSkypeHelper::AnalyzerSkypeAccount, folderRecord);
    }
    if(appName == L"table.mac.skypecontact"){
        return boost::bind(&CPSkypeHelper::AnalyzerSkypeContact,folderRecord);
    }
    if(appName == L"table.mac.skypemsg"){
        return boost::bind(&CPSkypeHelper::AnalyzerSkypeMsgs,folderRecord);
    }
    //mail
    if(appName == L"table.mac.AppleMailAccount"){
        CPAppleMailHelper helper;
        return boost::BOOST_BIND(&CPAppleMailHelper::AnalyzerMailAccount,helper,folderRecord);
    }
    if(appName == L"table.mac.AppleMailContact"){
        CPAppleMailHelper helper;
        return boost::BOOST_BIND(&CPAppleMailHelper::AnalyzerMailContact,helper,folderRecord);
    }
    if(appName == L"table.mac.AppleMailRecords"){
        CPAppleMailHelper helper;
        return boost::BOOST_BIND(&CPAppleMailHelper::AnalyzerMailRecords,helper,folderRecord);
    }
    if(appName == L"table.mac.ThunderbirdContact") {
        CPThunderBirdHelper thunderMail;
        return boost::BOOST_BIND(&CPThunderBirdHelper::AnalyzerMailContact,thunderMail,folderRecord);
    }
    if(appName == L"table.mac.ThunderbirdRecords"){
        CPThunderBirdHelper thunderMailHelp;
        return boost::BOOST_BIND(&CPThunderBirdHelper::AnalyzerMailRecords,thunderMailHelp,folderRecord);
    }
    //active task
    if(appName == L"table.mac.MacAddressBook"){
        AddressTaskHelper helper;
        return boost::BOOST_BIND(&AddressTaskHelper::GetAddressBookRecords,helper,folderRecord);//
    }
    if(appName == L"table.mac.RecentlyUsedItem"){
        RecentlyUsedItemhelper helper;
        return boost::BOOST_BIND(&RecentlyUsedItemhelper::GetRecentusedItemRecords,helper,folderRecord);
    }
    if(appName == L"table.mac.LogComputerBoot"){
        LogsTaskHelper cpHelp;
        return boost::BOOST_BIND(&LogsTaskHelper::GetComputerBootRecord,cpHelp,folderRecord);
    }
    if(appName == L"table.mac.LogDiskStatus"){
        LogsTaskHelper dailyHelp;
        return boost::BOOST_BIND(&LogsTaskHelper::GetDailyDiskRecords,dailyHelp,folderRecord);
    }
    if(appName == L"table.mac.LogNetworkInterface"){
        LogsTaskHelper netHelp;
        return boost::BOOST_BIND(&LogsTaskHelper::GetDailyNetwRecords,netHelp,folderRecord);
    }
    //user files
    if(appName == L"table.mac.LogFileInfo"){
        CPlogListAnalyzer fileListAnalyzer;
        return boost::BOOST_BIND(&CPlogListAnalyzer::getFileListInfos,fileListAnalyzer,folderRecord);
    }
    if(appName == L"table.mac.UserFileList"){
        CPlogListAnalyzer fileListAnalyzer;
        return boost::BOOST_BIND(&CPlogListAnalyzer::getFileListInfos,fileListAnalyzer,folderRecord);
    }
    if(appName == L"table.mac.EncryptFileInfo"){
        CPEncryptFileList fileEncryAnalyzer;
        return boost::BOOST_BIND(&CPEncryptFileList::getEncrypTFileList,fileEncryAnalyzer,folderRecord);
    }

    return nullptr;
}
}
