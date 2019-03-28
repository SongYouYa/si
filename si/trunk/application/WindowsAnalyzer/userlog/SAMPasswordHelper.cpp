#include "stdafx.h"
#include "siutility/globals.h"
#include "SAMPasswordHelper.h"
#include "OSLocater.h"
#include "siutility/PFileSiUtils.h"

namespace app
{

	bool SAMPasswordHelper::GetSAMPasswordInformation(SAMPasswordInformation& sam_password_information)
	{
		//get all files' path
		std::vector<std::pair<std::wstring, std::wstring> > picked_sam_system_files_;
		std::vector<boost::shared_ptr<WindowsOS>> all_windows_os;
		OSLocater oslocator;
		oslocator.GetAllOperationSystemInfo(all_windows_os);
		GetAllSAMFilesPathOfAllOS(all_windows_os, picked_sam_system_files_);
		// contract the system file, sam file from register
		try{
			bool issucess = PickupSAMFiles(picked_sam_system_files_);
			if (!issucess)
			{
				return false;
			}
		}
		catch (...)
		{
		}
		int tatalSize = picked_sam_system_files_.size();
		std::wstring samDir = siutil::CPDirHelper::get()->getTempDir();
		for (std::size_t i = 0; i < picked_sam_system_files_.size(); i++)
		{
			std::wstring samdir = picked_sam_system_files_.at(i).first;
			std::wstring systemDir = picked_sam_system_files_.at(i).second;
			std::wstring strCmd = siutil::CPDirHelper::get()->getToolDir() + L"\\pwdump\\pwdump.exe" + L" " + systemDir + L" " + samdir;
			if (!ParseToSAMPasswordInformation(strCmd.c_str(), sam_password_information)){
				continue;
			}
		}
		return true;
	}
	bool SAMPasswordHelper::ParseToSAMPasswordInformation(const std::wstring &cmd_path, SAMPasswordInformation& sam_password_information)
	{

		int exitCode = 0;
		std::string pipe_buf_;
		std::wstring exePath = cmd_path;
		if (!CPProcess::createProcess(exePath, L"", &pipe_buf_, &exitCode)){
			return false;
		}
		std::string strRecord = pipe_buf_;
		SAMPasswordRecord password_record;
		size_t nPosFirst = 0;
		size_t nPosNext = 0;
		const char *pszSplit = "\r\n";
		while (std::string::npos != (nPosNext = strRecord.find(pszSplit, nPosFirst)))
		{
			std::string strList(strRecord, nPosFirst, nPosNext - nPosFirst);
			nPosFirst = nPosNext + strlen(pszSplit);
			strList.erase(0, strList.find_first_not_of("\r\n\t "));
			if (strList.empty() || (strList.size() < 70)){ //排除命令执行出错结果
				break;
			}
			size_t nPosSplitBegin = 0;
			size_t nPosSplitEnd = strList.find(':', nPosSplitBegin);
			// User name
			password_record.user_name_ = GB2W(strList.substr(nPosSplitBegin, nPosSplitEnd - nPosSplitBegin));
			// User ID
			nPosSplitBegin = nPosSplitEnd + 1;
			nPosSplitEnd = strList.find(':', nPosSplitBegin);
			password_record.sid_ = GB2W(strList.substr(nPosSplitBegin, nPosSplitEnd - nPosSplitBegin));
			// Hash 1
			nPosSplitBegin = nPosSplitEnd + 1;
			nPosSplitEnd = strList.find(':', nPosSplitBegin);
			password_record.lm_hash_ = GB2W(strList.substr(nPosSplitBegin, nPosSplitEnd - nPosSplitBegin));
			// Hash 2
			nPosSplitBegin = nPosSplitEnd + 1;
			nPosSplitEnd = strList.find(':', nPosSplitBegin);
			password_record.nt_hash_ = GB2W(strList.substr(nPosSplitBegin, nPosSplitEnd - nPosSplitBegin));
			sam_password_information.push_back(password_record);
		} // End of WHILE nPosNext
		return true;
	}
	void SAMPasswordHelper::GetAllSAMFilesPathOfAllOS(const std::vector<boost::shared_ptr<WindowsOS>>& all_windows_os,
		std::vector<std::pair<std::wstring, std::wstring > >&sam_system_files)
	{
		for (size_t i = 0; i < all_windows_os.size(); i++)
		{
			boost::shared_ptr<WindowsOS> windows_info = all_windows_os.at(i);

			// XP:C:\WINDOWS\system32\config
			//win7: C:\Windows\System32\config
			if (windows_info->name_ == L"WindowsXP")
			{
				std::wstring sam_file = windows_info->GetSystemRegisterPath().append(L"\\SAM");
				std::wstring system_file = windows_info->GetSystemRegisterPath().append(L"\\system");
				sam_system_files.push_back(std::make_pair(sam_file, system_file));
			}
			else //if (windows_info.name_ == L"WindowsXP Later")
			{
				std::wstring sam_file = windows_info->GetSystemRegisterPath().append(L"/SAM");
				std::wstring system_file = windows_info->GetSystemRegisterPath().append(L"/system");
				sam_system_files.push_back(std::make_pair(sam_file, system_file));
			}
		}
	}
	bool SAMPasswordHelper::PickupSAMFiles(std::vector<std::pair<std::wstring, std::wstring > >&sam_system_files)
	{
		std::wstring tempDir = siutil::CPDirHelper::get()->getTempDir();
		tempDir += L"/samtemp";

		for (size_t i = 0; i < sam_system_files.size(); i++)
		{
			std::pair<std::wstring, std::wstring> sam_system_file = sam_system_files.at(i);
			boost::replace_all(sam_system_file.first, L"\\\\", L"/");
			boost::replace_all(sam_system_file.second, L"\\\\", L"/");
			int num = i;
			std::wstring wnumber = boost::lexical_cast<std::wstring>(num);
			std::wstring sam_file_name = L"sam" + wnumber;
			std::wstring system_file_name = L"system" + wnumber;
			std::wstring dest_sam = tempDir + L"\\" + sam_file_name;
			boost::replace_all(dest_sam, L"/", L"\\");
			std::wstring dest_system = tempDir + L"\\" + system_file_name;
			boost::replace_all(dest_system, L"/", L"\\");
			//添加一个复制文件的函数			
			siutil::CPSiFileUtils::exportFile(sam_system_file.first, dest_sam);
			siutil::CPSiFileUtils::exportFile(sam_system_file.second, dest_system);
			std::pair<std::wstring, std::wstring> sam_system_file_pair = make_pair(dest_sam, dest_system);
			sam_system_files.clear();
			sam_system_files.push_back(sam_system_file_pair);
		}

		return true;
	}

	DWORD SAMPasswordHelper::ReadFileContent(WCHAR *szFilePath)
	{
		HANDLE hFileRead;
		LARGE_INTEGER liFileSize;	//LONGLONG
		DWORD dwReadedSize;
		LONGLONG liRealyRead = 0;
		BYTE lpFileDataBuffer[32];
		hFileRead = CreateFile(szFilePath,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFileRead == INVALID_HANDLE_VALUE)
		{
			printf("Open File false: %d\n", GetLastError());
		}
		if (!GetFileSizeEx(hFileRead, &liFileSize))
		{
			printf("get File Size error! %d\n", GetLastError());
		}
		else
		{
			printf("the size of file is %d\n", liFileSize.QuadPart);
		}
		while (TRUE)
		{
			DWORD i;
			if (!ReadFile(hFileRead,
				lpFileDataBuffer,
				32,
				&dwReadedSize,
				NULL))
			{
				printf("read error! %d\n", GetLastError());
				break;
			}
			printf("read %d ,content is :", dwReadedSize);
			for (i = 0; i < dwReadedSize; i++)
			{
				printf("0x%x ", lpFileDataBuffer[i]);
			}
			printf("\n");
			liRealyRead += dwReadedSize;
			if (liRealyRead == liFileSize.QuadPart)
			{
				printf("File read finished\n");
				break;
			}
		}
		CloseHandle(hFileRead);
		return 0;
	}


	bool SAMPasswordHelper::analyze(CPRecord* parentRecord)
	{
		SAMPasswordInformation SAM_info;
		GetSAMPasswordInformation(SAM_info);
		if (SAM_info.empty()) {
			return false;
		}

		for (auto it = SAM_info.begin(); it != SAM_info.end(); it++)
		{
			SAMPasswordRecord& SAM_info = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"userName") = SAM_info.user_name_;
			record->at(L"SID") = SAM_info.sid_;
			record->at(L"LMHash") = SAM_info.lm_hash_;
			record->at(L"NTHash") = SAM_info.nt_hash_;

		}
		return true;
	}

	//void SAMPasswordHelper::GetLMPassword(wchar_t* cmd_path, SAMPasswordInformation& sam_password_information)
	//{
	//   /*if (!MemPipeParser::Parse(cmd_path))
	//    {
	//        return ;
	//    }*/
	//    std::string strRecord = pipe_buf_;
	//    SAMPasswordRecord password_record;
	//    size_t nPosFirst = 0;
	//    size_t nPosNext = 0;
	//    const char *pszSplit = "\r\n";
	//    while (std::string::npos != (nPosNext = strRecord.find(pszSplit, nPosFirst)))
	//    {
	//        std::string strList(strRecord, nPosFirst, nPosNext - nPosFirst);
	//        nPosFirst = nPosNext + strlen(pszSplit);
	//        strList.erase(0, strList.find_first_not_of("\r\n\t "));
	//        if (strList.empty())
	//        {
	//            continue;
	//        }
	//        size_t nPosSplitBegin = 0;
	//        size_t nPosSplitEnd = strList.find(':', nPosSplitBegin);
	//        // User name
	//        std::wstring user_name = StringToWstring(strList.substr(nPosSplitBegin, nPosSplitEnd - nPosSplitBegin));
	//        std::wstring lm_pass = L"";
	//        if (std::string::npos != strList.find("::"))
	//        {
	//             lm_pass = L"";
	//        }
	//        else
	//        {
	//            nPosSplitBegin = strList.find_first_not_of(' ', nPosSplitEnd+1);
	//            nPosSplitEnd = strList.find_first_of(':', nPosSplitBegin);
	//            lm_pass = StringToWstring(strList.substr(nPosSplitBegin, nPosSplitEnd - nPosSplitBegin));
	//        }
	//        for (size_t i = 0 ; i < sam_password_information.size(); i++)
	//        {
	//            SAMPasswordRecord record = sam_password_information.at(i);
	//            if (record.user_name_ == user_name)
	//            {
	//                record.lm_pass_ = lm_pass;
	//            }
	//        }
	//    } // End of WHILE nPosNext
	//}
}