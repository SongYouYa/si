#pragma once
#include "./globals.h"

namespace pfile
{
	class FILE_ANA_API CPLnkFile
	{
	public:
		CPLnkFile(PSIHANDLE handle);
		std::wstring targetPath();

	private:
		bool ParseFile();
		bool _parsed;
		std::wstring m_TargetPath;
		PSIHANDLE m_pFile;
	};
}
