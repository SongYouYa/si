#pragma once
#include "sisystem/win/PKnownFolder.h"

namespace app
{
	class CKnownFolderParser;
	class RecentRunProgAnalyzer
	{
	public:
		struct RecentRunProgram
		{
			std::wstring program_path_;	//程序路径
			CPTime run_time_;		//运行时间
			uint access_count_;	//访问次数
			RecentRunProgram()
				: access_count_(0)
			{

			}
		};
		typedef std::vector<RecentRunProgram> vecRecentRunInformaion;
		RecentRunProgAnalyzer(void);
		~RecentRunProgAnalyzer(void);

		bool analyze(CPRecord *rootRecord);
	private:
		//获取“RunMRU”键下的信息
		 void ParseRunMruRecords(vecRecentRunInformaion &vecInformation);
		//解析UserAssist键下的信息
		 bool ParseUserAssist(vecRecentRunInformaion& recentProgInfos);
		//解析读取到某个Value的数据（windows vista之后的系统版本）
		 bool ParseValueDataVistaLater(const std::wstring& valueName, const std::vector<byte>& valueData, RecentRunProgram& oneRecord);
		//解析windowsXp版本系统的Value数据
		 bool ParseValueDataXp(const std::wstring& valueName, const std::vector<byte>& valueData, RecentRunProgram& oneRecord);
		 //对数据进行“凯撒13”算法解密
		 static void DecodeRot13(TCHAR * tszBuffer, DWORD dwLen);
		 psystem::CPKnownFolder _kownFolder;
	};
}


