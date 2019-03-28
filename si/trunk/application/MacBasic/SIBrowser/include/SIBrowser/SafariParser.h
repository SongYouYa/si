#ifndef SIBROWSER_SAFARIPARSER_H
#define SIBROWSER_SAFARIPARSER_H

#include "BrowserInformation.h"
#include "BrowserBookmark.h"

namespace task {
    class TaskDelegate;
    class TaskStatusCtrl;
}

namespace  macbasic
{
    class CSafariParser
    {
    public:
        CSafariParser();
        ~CSafariParser();
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
        time_t  MAC_BASETIME;
    };
}

#endif // SAFARIPARSER_H
