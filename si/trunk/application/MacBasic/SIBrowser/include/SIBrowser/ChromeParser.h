#ifndef CHROMEPARSER_H
#define CHROMEPARSER_H

#include <string>
#include <boost/property_tree/ptree.hpp>
#include "BrowserInformation.h"
#include "BrowserBookmark.h"

namespace task {
    class TaskDelegate;
    class TaskStatusCtrl;
}

namespace  macbasic
{
    class CChromeParser
    {
    public:
        CChromeParser();
        ~CChromeParser();

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

#endif // CHROMEPARSER_H
