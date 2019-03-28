#include "stdafx.h"
#include "WindirTasksAnalyzer.h"
#include "filesystemshell/FileSystemShell.h"
using namespace filesystem;

namespace app
{
	CWindirTaskAnalyzer::CWindirTaskAnalyzer()
	{

	}

	CWindirTaskAnalyzer::~CWindirTaskAnalyzer()
	{

	}

	bool CWindirTaskAnalyzer::analyze(CPRecord *parentRecord)
	{
		WindirTaskInfos infos;
		if (!Parse(infos)){
			return false;
		}

		for (auto it = infos.begin(); it != infos.end(); ++it) {
			WindirTaskInfo& infos = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"Name") = it->name_;
			record->at(L"CreateTime") = it->createTime_;
			record->at(L"AccessTime") = it->accessTime_;
			record->at(L"ModifyTime") = it->modifyTime_; 
		}
		return true;
	}

	bool CWindirTaskAnalyzer::Parse( WindirTaskInfos& infos )
	{
		TCHAR tsEnvWindir[] = L"WINDIR";
		TCHAR tszWindir[MAX_PATH];
		::GetEnvironmentVariable(tsEnvWindir, tszWindir, MAX_PATH);
		std::wstring targetPath = tszWindir;
		std::string::size_type pos = targetPath.find(L":");
		if (pos == std::string::npos)
		{
			return false;
		}
		targetPath = targetPath.substr(pos + 1);
		targetPath += L"/Tasks/*.job";

		std::vector<PSIHANDLE> jobHandles;
		CFileSystem::GetItemHandlesByRawPath(targetPath, jobHandles);

		for (std::vector<PSIHANDLE>::iterator it = jobHandles.begin(); it != jobHandles.end(); ++it)
		{
			PSIHANDLE handle = *it;
			WindirTaskInfo oneInfo;
			oneInfo.name_ = CFileSystem::GetItemName(handle);
			oneInfo.createTime_ = CFileSystem::GetItemCreateTime(handle);
			oneInfo.accessTime_ = CFileSystem::GetItemLastAccessTime(handle);
			oneInfo.modifyTime_ = CFileSystem::GetItemModfiyTime(handle);
			infos.push_back(oneInfo);
		}
		return true;
	}

}