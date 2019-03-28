#include "stdafx.h"
#include "TemplateFile/PTemplateFile.h"
#include "TemplateApp.h"

#include "sicommon/PPath.h"

namespace tpfile
{
	std::wstring CPTemplateFile::s_templateDir;

	void CPTemplateFile::setTemplateDir(const std::wstring& dir)
	{
		s_templateDir = dir;
	}

	void CPTemplateFile::searchAllHandles(std::set<PSIHANDLE>& handles)
	{
		std::vector<std::wstring> templateNames;
		CPPath::getSubNames(s_templateDir, L".xml", templateNames, CPPath::FILTER_FILE);

		for (auto& tname : templateNames) {
			std::wstring tpath = s_templateDir + L"/" + tname;
			CTemplateApp tapp;
			if (!tapp.ParseTemplate(tpath)) {
				continue;
			}
			if (!tapp.SearchFiles()) {
				continue;
			}
			std::vector<PSIHANDLE> vhandles;
			tapp.GetSearchedHandles(vhandles);
			for (auto h : vhandles) {
				handles.insert(h);
			}
		}
	}
}