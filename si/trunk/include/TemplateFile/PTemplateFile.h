#pragma once
#include "./globals.h"

namespace tpfile
{
	class TPFILE_API CPTemplateFile
	{
	public:
		static void setTemplateDir(const std::wstring& dir);

		void searchAllHandles(std::set<PSIHANDLE>& handles);

	private:
		static std::wstring s_templateDir;
	};
}