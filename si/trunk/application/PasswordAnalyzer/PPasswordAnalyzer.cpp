#include "stdafx.h"
#include "application/PasswordAnalyzer/PPasswordAnalyzer.h"
#include "NirsoftPassword.h"
#include "MailPasswordsHelper.h"

namespace password
{
	bool CPPasswordAnalyzer::parsePassword(const std::wstring& typeName, CPRecord* parentRecord,
		CPTaskStatusCtrl* statusCtrl)
	{
		CNirsoftPassword nirPass;
		bool ret = nirPass.CrackNirsoftPassword(typeName, parentRecord, statusCtrl);

		if (typeName == L"table.MailPassword") {
			CMailPasswordsHelper mailHelper;
			mailHelper.GetAllMailPasswordsInformation(parentRecord);
		}
		return ret;
	}
}