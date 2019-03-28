#pragma once
#include <boost/property_tree/ptree.hpp>
#include "PBrowserInformation.h"
#include "PBrowserBookmark.h"

namespace  macbasic
{
    class CPChromeParser
    {
    public:
        CPChromeParser();
        ~CPChromeParser();

        bool GetHistory(CPRecord *rootRecord);
        bool GetDownload(CPRecord *rootRecord);
        bool GetCache(CPRecord *rootRecord);
        bool GetBookmark(CPRecord *rootRecord);
        bool GetCookie(CPRecord *rootRecord);
        bool GetForm(CPRecord *rootRecord);

    private:
        //history
        bool ParseHistory(std::wstring historyFile, BrowserHistorys& historys);
        //download
        bool ParseDownload(std::wstring downloadPlist, BrowserDownloads& downloads);
        //bookmark
        bool ParseBookmark(std::wstring bookmarkFile, BookMarkPtr bookmark);
        void BookmarkRecursive(boost::property_tree::wptree& pt, BookMarkPtr bookmark);
        //cookie
        bool ParseCookie(std::wstring cookieFile, BrowserCookies& cookies);
        //form
        bool ParseForm(std::wstring formFile, BrowserForms& forms);

    };
}
