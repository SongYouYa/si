#include "stdafx.h"
#include "application/PasswordAnalyzer/WindowsUserAccount.h"

#include "application/PTaskError.h"
#include "siutility/PFileHandler.h"

namespace password
{

	bool CWindowsUserAccountPassword::GetWindowsUserAccountInformation(CPRecord* parentRecord,
		CPTaskStatusCtrl* statusCtrl)
	{
		std::wstring toolsDir = siutil::CPDirHelper::get()->getToolDir();

		std::wstring toolPath = toolsDir + L"\\";
		toolPath += PASSWORD_DIR_NAME;
		if (psystem::CPOsVersion::isWin64()) {
			toolPath += L"\\mimikatz_x64.exe";
		}
		else {
			toolPath += L"\\mimikatz_win32.exe";
		}

		try {
			if (!boost::filesystem::exists(toolPath)) {
				std::wstring toolDir = siutil::CPDirHelper::get()->getToolDir();
				std::wstring passwordDir = toolDir + L"\\password";
				std::wstring zipPath = passwordDir + L"\\mimikatz.7z";
				if (!siutil::CPFileHandler::unzip(zipPath, passwordDir)) {
					return false;
				}
			}
		}
		catch (...) {
			return false;
		}

		return ParseToSAMPasswordInformation(toolPath, parentRecord, statusCtrl);
	}
	bool CWindowsUserAccountPassword::ParseToSAMPasswordInformation(std::wstring cmd_path,
		CPRecord* parentRecord, CPTaskStatusCtrl* statusCtrl)
	{
		std::string output;
		if (!CPProcess::createProcess(cmd_path, L"", &output)) {
			statusCtrl->fail(TASK_ERROR_TOOL_LOST);
			return false;
		}

		size_t nPosFirst = 0;
		size_t nPosNext = 0;
		const char *pszSplit = "\r\n";

		WindowsAccountInfo sam_record;

		while (std::string::npos != (nPosNext = output.find(pszSplit, nPosFirst)) || std::string::npos != (output.find("=", nPosFirst)))
		{
			std::string strList(output, nPosFirst, nPosNext - nPosFirst);

			if (std::string::npos != nPosNext)
			{
				nPosFirst = nPosNext + strlen(pszSplit);
			}
			else
			{
				nPosFirst = nPosNext;
			}

			strList.erase(0, strList.find_first_not_of("\r\n\t "));

			if (strList.empty())
			{
				continue;
			}

			if (strList.find("----------") != std::string::npos)
			{
				continue;
			}

			if (strList.find('=') == std::string::npos )
			{
				continue;
			}

			size_t nPosSplitBegin = strList.find_first_of('=', 0);

			if (strList.find("username=") != std::string::npos)
			{
				sam_record.user_name_ = LS2W(strList.substr(nPosSplitBegin+1));
				continue;
			}

			if (strList.find("lm=") != std::string::npos && strList.find("ntlm=") == std::string::npos)
			{
				sam_record.lm_hash_ = LS2W(strList.substr(nPosSplitBegin + 1));
				continue;
			}

			if (strList.find("ntlm=") != std::string::npos)
			{
				sam_record.nt_hash_ = LS2W(strList.substr(nPosSplitBegin + 1));
				continue;
			}

			if (strList.find("Package=") != std::string::npos)
			{
				sam_record.package_ = LS2W(strList.substr(nPosSplitBegin + 1));
				continue;
			}

			if (strList.find("domain=") != std::string::npos)
			{
				sam_record.domain_ = LS2W(strList.substr(nPosSplitBegin + 1));
				continue;
			}

			if (strList.find("password=") != std::string::npos)
			{
				sam_record.lm_pass_ = LS2W(strList.substr(nPosSplitBegin + 1));

				if (!(sam_record.lm_pass_.empty() && sam_record.lm_hash_.empty() && sam_record.nt_hash_.empty()))
				{
					//找到password，则一行信息已解析完
					CPRecord* record = parentRecord->addChild();
					record->at(L"UserName") = sam_record.user_name_;
					record->at(L"Password") = sam_record.lm_pass_;
					record->at(L"Package") = sam_record.package_;
					record->at(L"Domain") = sam_record.domain_;
					record->at(L"lm") = sam_record.lm_hash_;
					record->at(L"ntlm") = sam_record.nt_hash_;

					//如果和之前的相同，则删除
					std::vector<std::wstring> columnList;
					columnList.push_back(L"UserName");
					columnList.push_back(L"Password");
					columnList.push_back(L"Package");

					siutil::CPRecordHandler::removeLastSameChilren(parentRecord, columnList);
				}

				sam_record.user_name_.clear();
				sam_record.lm_hash_.clear();
				sam_record.lm_pass_.clear();
				sam_record.nt_hash_.clear();
			}

		} // End of WHILE nPosNext*/

		return true;
	}
}