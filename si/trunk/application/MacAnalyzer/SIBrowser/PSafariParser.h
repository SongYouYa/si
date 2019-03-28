#pragma once
#include "PBrowserInformation.h"
#include "PBrowserBookmark.h"

namespace  macbasic
{
    #define MAC_BASETIME 978307200 //2001/1/1 0:0:0

    class CPSafariParser
    {
    public:
        CPSafariParser();
        ~CPSafariParser();
        bool GetHistory(CPRecord *rootRecord);
        bool GetDownload(CPRecord *rootRecord);
        bool GetCache(CPRecord *rootRecord);
        bool GetBookmark(CPRecord *rootRecord);
        bool GetCookie(CPRecord *rootRecord);

    private:
        //history
        bool ParsePlistHistory(std::wstring historyFile, BrowserHistorys& historys); //safari 6.0
        bool ParseDbHistory(std::wstring historyFile, BrowserHistorys& historys);  //safari 8.0
        //download
        bool ParseDownload(std::wstring downloadPlist, BrowserDownloads& downloads);
        //cache
        bool ParseCache(std::wstring cacheFile, BrowserCaches& caches);
        //bookmark
        bool ParseBookmark(std::wstring bookmarkFile, BookMarkPtr bookmark);
        void BookmarkParseRecursive(plistcpp::dictionary_type& dict, BookMarkPtr bookmark);
        //cookie
        bool ParseCookie(std::wstring cookieFile, BrowserCookies& cookies);

    private:
        static time_t TimestrToTimet(const unsigned char* strTime);

    };
}

