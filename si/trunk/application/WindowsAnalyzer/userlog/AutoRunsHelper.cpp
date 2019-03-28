#include "stdafx.h"
#include "AutoRunsHelper.h"
#include <fstream>
#include <stdio.h>
namespace app
{
	AutoRunsHelper::AutoRunsHelper(void)
	{
	}


	AutoRunsHelper::~AutoRunsHelper(void)
	{
	}
	bool AutoRunsHelper::GetAutoRunsInformation(vecAutoRunsInformation &vecAutoruns)
	{
		std::wstring os_tools_dir = L"\"" + siutil::CPDirHelper::get()->getToolDir() + L"\"";
		std::wstring os_temp_dir = siutil::CPDirHelper::get()->getTempDir();
		std::wstring wstrXmlPath = os_temp_dir + L"/" + L"autorunsoutput.txt";
		std::wstring executeFilePath = L"cmd.exe /c " + os_tools_dir +L"\\userlog\\autorunsc.exe /accepteula -a -c > %s";
		WCHAR  cmd_line[MAX_PATH] = L"";
		swprintf(cmd_line, executeFilePath.c_str(),wstrXmlPath.c_str());
		std::string pipe_buf_;
		int exitCode = 0;
		if (!CPProcess::createProcess(cmd_line, L"", &pipe_buf_, &exitCode)){
			return false;
		}		
		std::ifstream examplefile(wstrXmlPath);
		if (!examplefile.is_open())
		{
			return false;
		}
		int i = 0;
		std::string strList;
		while (!examplefile.eof())
		{
			std::getline(examplefile, strList);
			AutoRunsInformation single_record;
			boost::char_separator<char> sep(",", NULL, boost::keep_empty_tokens);

			typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
			Tokenizer tokens(strList, sep);

			single_record.clear();

			for (Tokenizer::iterator it = tokens.begin(); it != tokens.end(); it++)
			{
				if ((*it).size() == 0)
					single_record.push_back(std::wstring(L""));
				else
				{
					std::wstring strTmp = GB2W(*it);
					single_record.push_back(GB2W(*it));
				}
			}
			if (i != 0){
				vecAutoruns.push_back(single_record);
			}
			i++;

		} // End of WHILE nPosNext



		DeleteFile(wstrXmlPath.c_str());
		return true;
	}
	bool AutoRunsHelper::analyze(CPRecord *parentRecord)
	{
		vecAutoRunsInformation vecAutoruns;
		GetAutoRunsInformation(vecAutoruns);
		if (vecAutoruns.empty()) {
			return false;
		}
		for (auto it = vecAutoruns.begin(); it != vecAutoruns.end()-1; ++it)
		{
			CPRecord* record = parentRecord->addChild();
			record->at(L"EntryLocation") = it->at(ENTRYLOCATION);
			record->at(L"Entry") = it->at(ENTRY);
			record->at(L"Enabled") = it->at(ENABLED);
			record->at(L"Description") = it->at(DESCRIPTION);
			record->at(L"Publisher") = it->at(PUBLISHER);
			record->at(L"ImagePath") = it->at(IMAGEPATH);
			record->at(L"LaunchString") = it->at(LAUNCHSTRING);
			record->at(L"MD5") = it->at(MDSTRING);
			record->at(L"SHA1") = it->at(SHA1STRING);
			record->at(L"SHA256") = it->at(SHA256STRING);
		}
		return true;
	}
}
