#include "stdafx.h"
#include "application/WindowsAnalyzer/PWinCallbackFactory.h"

#include "./basic/POsBaseParser.h"
#include "./basic/PApplistParser.h"

#ifdef OS_PWINDOWS
#include "application/PasswordAnalyzer/WindowsUserAccount.h"
#include "application/PasswordAnalyzer/PPasswordAnalyzer.h"

#include "./userlog/LogonSessionsInformationHelper.h"
#include "./userlog/PLoginUserHelper.h"
#include "./userlog/ProcessInfoHelper.h"
#include "./userlog/ModulesReliedByProcessHelper.h"
#include "./userlog/ServiceInfoHelper.h"
#include "./userlog/ProcessHandleHelper.h"
#include "./userlog/ClipboardInformationHelper.h"
#include "./userlog/ListDirTasksHelper.h"
#include "./userlog/SAMPasswordHelper.h"
#include "./userlog/AutoRunsHelper.h"
#include "./userlog/AccessShortCutHelper.h"
#include "./userlog/RecentRunProgAnalyzer.h"
#include "./userlog/LogonListInfoHelper.h"
#include "./userlog/WindirTasksAnalyzer.h"
#include "./userlog/PRecentFilesAnalyzer.h"

#include "./net/NetUseHelper.h"
#include "./net/PortInformationHelper.h"
#include "./net/PromiscPortHelper.h"
#include "./net/RouterInformationHelper.h"
#include "./net/NetFilesInformationHelper.h"
#include "./net/NetProtocolStatisticsHelper.h"
#include "./net/NetShareSourceInfoHelper.h"
#include "./net/ARPInfoHelper.h"
#include "./net/NetSessionsHelper.h"
#include "./net/NetBiosStatHelper.h"
#include "./net/NetActiveStatHelper.h"
#include "./net/NetworkInterfaceHelper.h"
#include "./net/NetAccountsParser.h"
#endif

namespace app
{
	AppCallback CPWinCallbackFactory::getCallback(PSIHANDLE volume, CPRecord* folderRecord,
		CPTaskStatusCtrl* statusCtrl)
	{
		std::wstring appName = folderRecord->getType()->name();

		//os common
		if (appName == L"table.win.osinfo") {
			return std::bind(CPOsBaseParser::getOsBaseInfoRecord, volume, folderRecord);
		}
		else if (appName == L"table.win.applist_sys") {
			return std::bind(CPApplistParser::getApplist, volume, folderRecord, true);
		}
		else if (appName == L"table.win.applist_user") {
			return std::bind(CPApplistParser::getApplist, volume, folderRecord, false);
		}
#ifdef OS_PWINDOWS
		//password
		else if (appName == L"table.win.UserAccountPassword") {
			return std::bind(password::CWindowsUserAccountPassword::GetWindowsUserAccountInformation,
				folderRecord, statusCtrl);
		}
		else if (appName == L"table.win.BrowserPassword" ||
			appName == L"table.win.MailPassword" ||
			appName == L"table.win.BulletPassword" ||
			appName == L"table.win.MailPassword" ||
			appName == L"table.win.IMPassword" ||
			appName == L"table.win.OutlookPstPassword" ||
			appName == L"table.win.VNCPassword" ||
			appName == L"table.win.WirelessPassword" ||
			appName == L"table.win.DialupVPNPassword") {
			return std::bind(password::CPPasswordAnalyzer::parsePassword, appName, folderRecord, statusCtrl);
		}
		//user log
		else if (appName == L"table.win.login_session") {
			return std::bind(LogonSessionsInformationHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.current_loginuser") {
			return std::bind(CPLoginUserHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.process_info") {
			return std::bind(ProcessInfoHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.process_modules") {
			return std::bind(ModulesReliedByProcessHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.service_info") {
			return std::bind(ServiceInfoHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.openedfile_handle") {
			return std::bind(ProcessHandleHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.clipboard") {
			return std::bind(ClipboardInformationHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.windows_task") {
			return std::bind(ListDirTasksHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.samhash") {
			return std::bind(SAMPasswordHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.autorun") {
			return std::bind(AutoRunsHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.accessed_shortcut") {
			return std::bind(AccessShortcutHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.recent_runprogram") {
			RecentRunProgAnalyzer analyzer;
			return std::bind(&RecentRunProgAnalyzer::analyze, analyzer, folderRecord);
		}
		else if (appName == L"table.win.login_log") {
			return std::bind(&LogonListInfoHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.NetAccounts") {	
		    return std::bind(&CNetAccountsParser::analyze, folderRecord);
		}
		
		else if (appName == L"table.win.windirtasks") {
			return std::bind(&CWindirTaskAnalyzer::analyze, folderRecord);
		}
		else if (appName == L"table.win.recent_openedfile") {
			return std::bind(&CPRecentFilesAnalyzer::analyze, folderRecord);
		}
		//net
		else if (appName == L"table.win.netconnection") {
			return std::bind(&NetUseHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.tcpipport") {
			PortInformationHelper analyzer;
			return std::bind(&PortInformationHelper::analyze, analyzer, folderRecord);
		}
		else if (appName == L"table.win.NetInterface") {
			return std::bind(&NetworkInterfaceHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.promiscport") {
			return std::bind(&PromiscPortHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.routerinfo") {
			return std::bind(&RouterInformationHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.netfile") {
			return std::bind(&NetFilesInformationHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.netprotocol_statics") {
			NetProtocolStatisticsHelper analyzer;
			return std::bind(&NetProtocolStatisticsHelper::analyze, analyzer, folderRecord);
		}
		else if (appName == L"table.win.netshare_source") {
			return std::bind(&NetShareSourceInfoHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.arp_statics") {
			return std::bind(&ARPInformationHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.netsessions") {
			return std::bind(&NetSessionsHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.netbios_state") {
			return std::bind(&NetBiosStatHelper::analyze, folderRecord);
		}
		else if (appName == L"table.win.netactivatestate") {
			return std::bind(&NetActiveStatHelper::analyze, folderRecord);
		}
#endif

		return nullptr;
	}
}
