#include "stdafx.h"
#include "ProcessHandleHelper.h"
#include "utility/PFileUtils.h"

namespace app
{
	bool ProcessHandleHelper::ParseProcessHandleListInformation(ProcessHandleInformation& process_handles_info)
	{
		std::wstring toolPath = siutil::CPDirHelper::get()->getToolDir();
		toolPath += L"/userlog/handle.exe";
		toolPath = L"\"" + toolPath + L"\"";
		//输出数据较多时，把输出导出到文件比直接获取输出要快很多
		std::wstring outFile = siutil::CPDirHelper::get()->getTempDir() + L"/temphandle.txt";
		std::wstring cmd = toolPath + L" /accepteula";
		if (!CPProcess::createProcess(cmd, L"", outFile)){
			return false;
		}
		std::string content;
		if (!util::CPFileUtils::readContent(outFile, content)) {
			return false;
		}
		::remove(W2LS(outFile).c_str());

		char* retBuf = (char*)content.c_str();
		char seps[] = "\r\n";
		char *token, *next_token;
		char sSep[] = "------------------------------------------------------------------------------";
		// Establish string and get the first token:
		token = strtok_s(retBuf, seps, &next_token);

		while (token != NULL)
		{
			if (strcmp(token, sSep) == 0)
			{
				break;
			}

			token = strtok_s(NULL, seps, &next_token);
		}

		// 单一条记录
		ProcessHandleRecord single_record;
		char szProcName[256];
		boost::uint32_t nPid;
		while (token != NULL)
		{
			if (strcmp(token, sSep) == 0)
			{
				if (single_record.process_name_ != L"")
				{
					process_handles_info.push_back(single_record);
					single_record.process_name_ = L"";
					single_record.handles_.clear();
				}

				token = strtok_s(NULL, seps, &next_token);	// Process Name;
				char sProcFmt[] = "%s pid: %d %s";
				char szBuffer[256];
				sscanf(token, sProcFmt, szProcName, &nPid, szBuffer);
				single_record.process_name_ = LS2W(szProcName);
				single_record.pid_ = nPid;
			}
			else
			{
				if (strstr(token, "File ") != NULL)
				{
					std::string tmp_string = token;
					size_t pos_split_begin = tmp_string.find_first_not_of(' ');
					size_t pos_split_end = tmp_string.find_first_of(':');
					std::string hid = tmp_string.substr(pos_split_begin, pos_split_end - pos_split_begin);
					pos_split_begin = tmp_string.find_first_not_of(' ', pos_split_end + 1);
					pos_split_end = tmp_string.find_first_of("  ", pos_split_begin);
					std::string type = tmp_string.substr(pos_split_begin, pos_split_end - pos_split_begin);
					pos_split_begin = tmp_string.find_first_not_of(' ', pos_split_end);
					pos_split_end = tmp_string.find_first_of("  ", pos_split_begin);
					std::string share_flags = "";
					if (std::string::npos != pos_split_begin)
					{
						share_flags = tmp_string.substr(pos_split_begin, pos_split_end - pos_split_begin);
					}

					pos_split_begin = tmp_string.find_first_not_of(' ', pos_split_end);
					std::string handle_name = "";

					if (std::string::npos != pos_split_begin)
					{
						handle_name = tmp_string.substr(pos_split_begin);
					}

					HandleInfo handle;
					handle.handle_name_ = LS2W(handle_name);
					handle.hid_ = LS2W(hid);
					handle.share_flags_ = LS2W(share_flags);
					handle.type_ = LS2W(type);
					single_record.handles_.push_back(handle);
				}
				else
				{
					std::string tmp_string = token;
					size_t pos_split_begin = tmp_string.find_first_not_of(' ');
					size_t pos_split_end = tmp_string.find_first_of(':');
					std::string hid = tmp_string.substr(pos_split_begin, pos_split_end - pos_split_begin);
					 
					pos_split_begin = tmp_string.find_first_not_of("  ", pos_split_end + 1);
					pos_split_end = tmp_string.find_first_of("  ", pos_split_begin);
					std::string type = "";

					if (std::string::npos != pos_split_begin)
					{
						type = tmp_string.substr(pos_split_begin, pos_split_end - pos_split_begin);
					}

					if (type == "ALPC")
					{
						type = "ALPC Port";
						pos_split_end = tmp_string.find("Port", pos_split_begin) + 4;
					}

					pos_split_begin = tmp_string.find_first_not_of(' ', pos_split_end);
					std::string handle_name = "";

					if (std::string::npos != pos_split_begin)
					{
						handle_name = tmp_string.substr(pos_split_begin);
					}

					HandleInfo handle;
					handle.handle_name_ = LS2W(handle_name);
					handle.hid_ = LS2W(hid);
					handle.share_flags_ = L"";
					handle.type_ = LS2W(type);
					single_record.handles_.push_back(handle);
				}
			}

			// process_handles_info.push_back(single_record);
			token = strtok_s(NULL, seps, &next_token);
		}

		/* delete []pipe_buf_;
		pipe_buf_= NULL;*/
		return true;
	}

	bool ProcessHandleHelper::analyze(CPRecord* parentRecord)
	{
		ProcessHandleInformation  ProcessHandleinfo;
		ParseProcessHandleListInformation(ProcessHandleinfo);
		if (ProcessHandleinfo.empty()) {
			return false;
		}

		for (auto it = ProcessHandleinfo.begin(); it != ProcessHandleinfo.end(); it++)
		{
			ProcessHandleRecord& ProcessHandleinfo = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"ProcessName") = ProcessHandleinfo.process_name_;	
			record->at(L"ProcessID") = ProcessHandleinfo.pid_;
			for (auto processHadhndl : ProcessHandleinfo.handles_){
				record->at(L"ShareFlag") = processHadhndl.share_flags_;
				record->at(L"Type") = processHadhndl.type_;
				record->at(L"HandleName") = processHadhndl.handle_name_;
				record->at(L"HandleID") = processHadhndl.hid_;
			}
				
		}
		return true;
	}
}
