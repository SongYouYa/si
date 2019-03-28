#pragma once
#include "./globals.h"

namespace password
{
	class PASS_API CPPasswordAnalyzer
	{
	public:
		static bool parsePassword(const std::wstring& typeName, CPRecord* parentRecord,
			CPTaskStatusCtrl* statusCtrl);
	};
}