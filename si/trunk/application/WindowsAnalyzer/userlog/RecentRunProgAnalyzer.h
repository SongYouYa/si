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
			std::wstring program_path_;	//����·��
			CPTime run_time_;		//����ʱ��
			uint access_count_;	//���ʴ���
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
		//��ȡ��RunMRU�����µ���Ϣ
		 void ParseRunMruRecords(vecRecentRunInformaion &vecInformation);
		//����UserAssist���µ���Ϣ
		 bool ParseUserAssist(vecRecentRunInformaion& recentProgInfos);
		//������ȡ��ĳ��Value�����ݣ�windows vista֮���ϵͳ�汾��
		 bool ParseValueDataVistaLater(const std::wstring& valueName, const std::vector<byte>& valueData, RecentRunProgram& oneRecord);
		//����windowsXp�汾ϵͳ��Value����
		 bool ParseValueDataXp(const std::wstring& valueName, const std::vector<byte>& valueData, RecentRunProgram& oneRecord);
		 //�����ݽ��С�����13���㷨����
		 static void DecodeRot13(TCHAR * tszBuffer, DWORD dwLen);
		 psystem::CPKnownFolder _kownFolder;
	};
}


