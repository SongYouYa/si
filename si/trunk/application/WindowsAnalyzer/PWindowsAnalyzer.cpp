#include "stdafx.h"
#include "application/WindowsAnalyzer/PWindowsAnalyzer.h"

#include "./basic/POsBaseParser.h"

#include "PReportFlags.h"
#include "utility/PSystemInfo.h"

namespace app
{
	//如果多线程同时读取文件系统，反而使总时间更慢，所以使用文件系统的应用，
	//应该获取此mutex，防止同时读取文件系统
	std::mutex g_fsMutex;

	std::vector<std::wstring> g_commonApps = {
		L"table.win.osinfo", L"table.win.applist_sys", L"table.win.applist_user"
	};

	std::vector<std::wstring> g_passwordApps = {
		L"table.win.UserAccountPassword", L"table.win.BrowserPassword", L"table.win.MailPassword",
		L"table.win.BulletPassword", L"table.win.IMPassword", L"table.win.OutlookPstPassword",
		L"table.win.VNCPassword", L"table.win.WirelessPassword", L"table.win.DialupVPNPassword"
	};

	std::vector<std::wstring> g_userLogApps = {
		L"table.win.login_session", L"table.win.current_loginuser", L"table.win.process_info",
		L"table.win.process_modules", L"table.win.service_info", L"table.win.openedfile_handle",
		L"table.win.clipboard", L"table.win.windows_task", L"table.win.samhash",
		L"table.win.autorun", L"table.win.accessed_shortcut", L"table.win.recent_runprogram",
		L"table.win.login_log", L"table.win.NetAccounts", L"table.win.windirtasks",
		L"table.win.recent_openedfile"
	};

	std::vector<std::wstring> g_netApps = {
		L"table.win.netconnection", L"table.win.tcpipport", L"table.win.NetInterface",
		L"table.win.promiscport", L"table.win.routerinfo", L"table.win.netfile",
		L"table.win.netprotocol_statics", L"table.win.netshare_source", L"table.win.arp_statics",
		L"table.win.netsessions", L"table.win.netbios_state", L"table.win.netactivatestate"
	};


	
    void addWinAppItems(CPRecord* parentFolder, const std::vector<std::wstring>& appTypes)
	{
		CPRecordTypeFactory* typeFactory = CPRecordTypeFactory::get();

		for (auto appName : appTypes) {
			CPRecord* appFolder = typeFactory->getType(appName)->newRecord();
			appFolder->addFlag(RFlagNoReport | RFlagNoShow);
			parentFolder->addChild(appFolder);
		}
	}

	CPWindowsAnalyzer::CPWindowsAnalyzer()
	{

	}

	void CPWindowsAnalyzer::addCommonApps(CPRecord* sysFolder)
	{
        addWinAppItems(sysFolder, g_commonApps);
	}
	void CPWindowsAnalyzer::addOnlineApps(CPRecord* sysFolder)
	{
		CPRecordTypeFactory* typeFactory = CPRecordTypeFactory::get();
		CPRecord* appfolder = typeFactory->getType(L"table.win.password_apps")->newRecord();
		appfolder->addFlag(RFlagNoReport | RFlagNoShow);
		sysFolder->addChild(appfolder);
        addWinAppItems(appfolder, g_passwordApps);

		appfolder = typeFactory->getType(L"table.win.user_log_apps")->newRecord();
		appfolder->addFlag(RFlagNoReport | RFlagNoShow);
		sysFolder->addChild(appfolder);
        addWinAppItems(appfolder, g_userLogApps);

		appfolder = typeFactory->getType(L"table.win.net_apps")->newRecord();
		appfolder->addFlag(RFlagNoReport | RFlagNoShow);
		sysFolder->addChild(appfolder);
        addWinAppItems(appfolder, g_netApps);
	}

	void CPWindowsAnalyzer::initRawApps(CPRecord* rootRecord)
	{
		CPRecordTypeFactory* typeFactory = CPRecordTypeFactory::get();
		CPRecordType* sysFolderType = typeFactory->getType(L"table.systemfolder");
		CPRecord* sysFolder = sysFolderType->newRecord();
		rootRecord->addChild(sysFolder);
		sysFolder->setNodeName(L"Windows OS");
#ifdef OS_PWINDOWS
		addCommonApps(sysFolder);
		addOnlineApps(sysFolder);
#elif OS_PLINUX
		addCommonApps(sysFolder);
#endif
	}

	void CPWindowsAnalyzer::initApps(CPRecord* rootRecord)
	{
		CPRecordTypeFactory* typeFactory = CPRecordTypeFactory::get();

		//系统信息
		CPRecordType* sysFolderType = typeFactory->getType(L"table.systemfolder");

		std::wstring systemDrive = util::CPSysteminfo::getSystemDrive();

		std::vector<PSIHANDLE> pVolumes;
		CFileSystem::GetAllVolumes(pVolumes);
		for (PSIHANDLE volume : pVolumes) {
			OsBaseInfo osBaseInfo;
			if (!CPOsBaseParser::getOsBaseInfo(volume, osBaseInfo)) {
				continue;
			}
			CPRecord* sysFolder = sysFolderType->newRecord();
			sysFolder->addFlag(RFlagNoReport | RFlagNoShow);
			rootRecord->addChild(sysFolder);
			sysFolder->setNodeName(osBaseInfo.computerName + L"-" + osBaseInfo.osProductName);
			sysFolder->at(L"pvolume") = volume;

            addWinAppItems(sysFolder, g_commonApps);

#ifdef OS_PWINDOWS
			if (osBaseInfo.systemRoot.find(systemDrive) == std::string::npos) {
				continue;
			}
			//当前在线系统才获取下面的app
			addOnlineApps(sysFolder);
#endif
		}
	}
}
