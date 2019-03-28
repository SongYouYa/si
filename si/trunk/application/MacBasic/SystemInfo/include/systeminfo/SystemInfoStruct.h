#ifndef SYSTEMINFO_SYSTEMINFOSTRUCT_H
#define SYSTEMINFO_SYSTEMINFOSTRUCT_H

#include "Common.h"
#include "base/PRecord.h"
#include <vector>
#include <map>

namespace systeminfo
{
	//安装多个系统时，用来识别系统的参数
	struct OsIdentifyParam
	{
		std::wstring computerName_; //计算机名
		base::CPTime installDate_; //安装时间
		std::wstring systemRootDir_; //系统主目录

		void AddToTable(base::CPRecordTable* table)
		{
			uint64 idx = table->AppendOne();
			table->at(idx, L"ComputerName") = computerName_;
			table->at(idx, L"SystemRoot") = systemRootDir_;
			table->at(idx, L"InstallDate") = installDate_;
		}
	};
	typedef std::vector<OsIdentifyParam> OsIdentifyParams;

	struct WOsBaseInfo
	{
		std::wstring computer_name_;
		std::wstring kernel_version_;
		std::wstring product_type_;
		std::wstring service_version_;
		std::wstring product_version_;
		std::wstring build_number_;
		std::wstring reg_owner_;
		std::wstring reg_org_;
		std::wstring ie_version_;
		std::wstring system_root_;
		base::CPTime install_date_;

		void AddToTable(base::CPRecordTable* table)
		{
			uint64 idx = table->AppendOne();
			table->at(idx, L"ComputerName") = computer_name_;
			table->at(idx, L"SystemVersion") = kernel_version_;
			table->at(idx, L"ProductVersion") = product_version_;
			table->at(idx, L"ServerVersion") = service_version_;
			table->at(idx, L"IEVersion") = ie_version_;
			table->at(idx, L"RegisterOwner") = reg_owner_;
			table->at(idx, L"SystemRoot") = system_root_;
			table->at(idx, L"ServerVersion") = service_version_;
			table->at(idx, L"InstallDate") = install_date_;
		}
	};
	typedef std::vector<WOsBaseInfo> WOsBaseInfos;

	struct WSoftwareInfo
	{
		std::wstring display_name_;
		std::wstring display_version_;
		std::wstring publisher_;
		std::wstring uninstall_string_;
		std::wstring url_info_about_;
		int type_;

		void AddToTable(base::CPRecordTable* table, std::wstring computerName)
		{
			uint64 idx = table->AppendOne();
			table->at(idx, L"DisplayName") = display_name_;
			table->at(idx, L"Publisher") = publisher_;
			table->at(idx, L"DisplayVersion") = display_version_;
			table->at(idx, L"AboutUrl") = url_info_about_;
			table->at(idx, L"sysOwner") = computerName;
		}
	};
	typedef std::vector<WSoftwareInfo> WSoftwareInfos;
	typedef std::map<time64_t, WSoftwareInfos> WAllOsSoftwareInfos; //map（系统安装时间，对应系统安装的应用软件）
}

#endif