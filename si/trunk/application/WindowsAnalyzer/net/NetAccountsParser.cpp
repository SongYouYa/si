#include "stdafx.h"
#include "NetAccountsParser.h"

namespace app
{
	CNetAccountsParser::CNetAccountsParser()
	{

	}
	CNetAccountsParser::~CNetAccountsParser()
	{

	}

	bool CNetAccountsParser::Parse(NetAccountsInfos& infos)
	{
		std::wstring executeFilePath = L"net.exe  accounts ";
		std::string pipe_buf_;
		int exitCode = 0;
		if (!CPProcess::createProcess(executeFilePath, L"", &pipe_buf_, &exitCode)){
			return false;
		}
		std::vector<std::string> vStr;
		std::vector<std::string> vStrSingle;
		boost::split(vStr, pipe_buf_, boost::is_any_of("\r\n"), boost::token_compress_on);
		for (std::vector<std::string>::iterator it = vStr.begin(); it != vStr.end(); ++it)
		{
			NetAccountsInfo oneInfo;
			boost::split(vStrSingle, *it, boost::is_any_of(":"), boost::token_compress_on);
			if (vStrSingle.size() != 2){
				break;
			}		
			oneInfo.itemName_ = LS2W(vStrSingle.at(0));
			std::string itevalue = vStrSingle.at(1);
			boost::trim(itevalue);
			oneInfo.itemValue_ = LS2W(itevalue);
			infos.push_back(oneInfo);
		}
		return true;
	}

	bool CNetAccountsParser::analyze(CPRecord* parentRecord)
	{
		NetAccountsInfos infos;
		if (!Parse(infos)){
			return false;
		}
		for (NetAccountsInfos::iterator it = infos.begin(); it != infos.end(); ++it)
		{
			CPRecord* record = parentRecord->addChild();
			record->at(L"Name") = it->itemName_;
			record->at(L"Value") = it->itemValue_;
		}
		return true;
	}

}