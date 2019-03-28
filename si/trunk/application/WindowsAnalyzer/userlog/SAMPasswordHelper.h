#pragma once
#include "./OSLocater.h"

namespace app
{
	class  SAMPasswordHelper 
	{
	public:
		struct SAMPasswordRecord
		{
			std::wstring user_name_;
			std::wstring sid_;
			std::wstring lm_hash_;
			std::wstring nt_hash_;
			std::wstring lm_pass_;
		};
		typedef std::vector<SAMPasswordRecord>  SAMPasswordInformation;

		static bool GetSAMPasswordInformation(SAMPasswordInformation& sam_password_information);
		static bool analyze(CPRecord* rootRecord);
		static DWORD ReadFileContent(WCHAR *szFilePath);
	private:
		static void GetAllSAMFilesPathOfAllOS(const std::vector<boost::shared_ptr<WindowsOS>>& all_windows_os, std::vector<std::pair<std::wstring, std::wstring > >&sam_system_files);
		static bool PickupSAMFiles(std::vector<std::pair<std::wstring, std::wstring > >&sam_system_files);
		static bool ParseToSAMPasswordInformation(const std::wstring &cmd_path, SAMPasswordInformation& sam_password_information);
		static void GetLMPassword(wchar_t* cmd_path, SAMPasswordInformation& sam_password_information);
		std::vector<std::pair<std::wstring, std::wstring> > picked_sam_system_files_;
	};
}

