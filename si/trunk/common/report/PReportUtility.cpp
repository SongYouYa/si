#include "stdafx.h"
#include "report/PReportUtility.h"
#include "sicommon/PPath.h"

namespace report
{
	bool CPReportUtility::OpenHtml(const std::wstring& htmlPath)
	{
		std::wstring forPath = htmlPath;
        CPPath::formatPathSlash(forPath);
#ifdef _MSC_VER
		const std::wstring urlPrefix = L"file:///";
		std::wstring htmlUrl = urlPrefix + forPath;
		HINSTANCE hinstance = ShellExecute(NULL, L"open", htmlUrl.c_str(), NULL, NULL, SW_SHOWNORMAL);
		if ((int)hinstance <= 32) {
			return false;
		}
		return true;
#elif OS_PAPPLE
        std::string openCmd = "open -a safari \"" + W2LS(htmlPath) + "\"";
        int ret = ::system(openCmd.c_str());
        return ret == 0;
#else
		return false;
#endif
	}

}
