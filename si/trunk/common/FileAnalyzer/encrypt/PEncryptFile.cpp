#include "stdafx.h"
#include "FileAnalyzer/PEncryptFile.h"
#include "CheckHelper.h"

#include "siutility/FileSearch.h"

namespace pfile
{
	CPEncryptFile::CPEncryptFile()
	{

	}

	void CPEncryptFile::getAllEncryptHandles(std::set<PSIHANDLE>& handles)
	{
		std::vector<PSIHANDLE> pItems;
		siutil::CFileSearch fs(L"", L"", L".doc|.ppt|.xls|.dmg");
		fs.GetHandles(pItems);

		for (auto it_i = pItems.begin(); it_i != pItems.end(); ++it_i) {
			PSIHANDLE hh = *it_i;
			if (CheckHelper::IsEncrypted(hh)) {
				handles.insert(hh);
			}
		}
	}
}