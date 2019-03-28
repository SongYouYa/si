#include "stdafx.h"
#include "PSafariParser.h"
#include "PSafariCookieDef.h"
#include "siutility/PMacFileExport.h"

using namespace filesystem;

namespace  macbasic
{

    CPSafariParser::CPSafariParser()
    {
    }

    CPSafariParser::~CPSafariParser()
    {

    }

    bool CPSafariParser::ParsePlistHistory(std::wstring historyFile, BrowserHistorys& historys)
    {
        std::string strFilePath = CPCodeChanger::UnicodeToUTF8(historyFile.c_str());
        plistcpp::dictionary_type dict;
        try
        {
            plistcpp::CPlistCpp::ReadPlist(strFilePath.c_str(), dict);
        }
        catch(...)
        {
            return false;
        }

        plistcpp::dictionary_type::iterator it_d = dict.find("WebHistoryDates");
        if(it_d == dict.end())
        {
            return false;
        }
        plistcpp::array_type& arr = boost::any_cast<plistcpp::array_type&>(it_d->second);
        plistcpp::array_type::iterator it_a = arr.begin();
        for(; it_a != arr.end(); ++it_a)
        {
            CPBrowserHistory newHistory;
            plistcpp::dictionary_type& dict_c = boost::any_cast<plistcpp::dictionary_type&>(*it_a);
            plistcpp::dictionary_type::iterator it_dc = dict_c.find("");
            if(it_dc != dict_c.end())
            {
                std::string strUrl = boost::any_cast<std::string>(it_dc->second);
                newHistory.url_ = CPCodeChanger::UTF8ToUnicode(strUrl.c_str());
            }
            it_dc = dict_c.find("lastVisitedDate");
            if(it_dc != dict_c.end())
            {
                std::string appTime = boost::any_cast<std::string>(it_dc->second);
                double dapptime = strtod(appTime.c_str(), NULL);
                time_t ttime = MAC_BASETIME + dapptime;
                newHistory.visit_time_ = CPTime::UTC2LocalTime(ttime, siutil::MacCommonHelper::GetTimezoneRegion());
            }
            it_dc = dict_c.find("title");
            if(it_dc != dict_c.end())
            {
                std::string strTitle = boost::any_cast<std::string>(it_dc->second);
                newHistory.title_ = CPCodeChanger::UTF8ToUnicode(strTitle.c_str());
            }
            it_dc = dict_c.find("visitCount");
            if(it_dc != dict_c.end())
            {
                newHistory.visit_count_ = boost::any_cast<plistcpp::integer_type>(it_dc->second);
            }
            historys.push_back(newHistory);
        }

        return true;
    }

    bool CPSafariParser::ParseDbHistory(std::wstring historyFile, BrowserHistorys& historys)
    {
        std::string strFile = CPCodeChanger::UnicodeToUTF8(historyFile.c_str());
        sqlite3* db = NULL;

        if(sqlite3_open(strFile.c_str(), &db) != SQLITE_OK)
        {
            return false;
        }
        std::string query = "select history_visits.title, history_items.url, history_visits.visit_time from \
                history_items, history_visits where history_visits.history_item = history_items.id order by history_visits.visit_time DESC";
        sqlite3_stmt *stmt = NULL;
        if(sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) != SQLITE_OK)
        {
            return false;
        }
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            CPBrowserHistory newHistory;
            newHistory.title_ = CPCodeChanger::UTF8ToUnicode((char*)sqlite3_column_text(stmt, 0));
            newHistory.url_ = CPCodeChanger::UTF8ToUnicode((char*)sqlite3_column_text(stmt, 1));
            double visitTime = sqlite3_column_double(stmt, 2);
            double dVt = 0;
            modf(visitTime, &dVt);
            time_t tvt = dVt + MAC_BASETIME;
            newHistory.visit_time_ = CPTime::UTC2LocalTime(tvt, siutil::MacCommonHelper::GetTimezoneRegion());
            newHistory.visit_count_ = 1;
            historys.push_back(newHistory);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    bool CPSafariParser::ParseDownload(std::wstring downloadPlist, BrowserDownloads& downloads)
    {
        std::string strFilePath = CPCodeChanger::UnicodeToUTF8(downloadPlist.c_str());
        plistcpp::dictionary_type dict;
        try
        {
            plistcpp::CPlistCpp::ReadPlist(strFilePath.c_str(), dict);
        }
        catch(...)
        {
            return false;
        }

        plistcpp::dictionary_type::iterator it_d = dict.find("DownloadHistory");
        if(it_d == dict.end())
        {
            return false;
        }
        plistcpp::array_type& arr = boost::any_cast<plistcpp::array_type&>(it_d->second);
        plistcpp::array_type::iterator it_a = arr.begin();
        for(; it_a != arr.end(); ++it_a)
        {
            CPBrowserDownload newDownload;
            plistcpp::dictionary_type& dict_c = boost::any_cast<plistcpp::dictionary_type&>(*it_a);
            plistcpp::dictionary_type::iterator it_dc = dict_c.find("DownloadEntryPath");
            if(it_dc != dict_c.end())
            {
                std::string strPath = boost::any_cast<std::string>(it_dc->second);
                newDownload.savePath_ = CPCodeChanger::UTF8ToUnicode(strPath.c_str());
            }
            it_dc = dict_c.find("DownloadEntryURL");
            if(it_dc != dict_c.end())
            {
                std::string strUrl = boost::any_cast<std::string>(it_dc->second);
                newDownload.url_ = CPCodeChanger::UTF8ToUnicode(strUrl.c_str());
            }
            it_dc = dict_c.find("DownloadEntryProgressBytesSoFar");
            if(it_dc != dict_c.end())
            {
                int loadsize = boost::any_cast<plistcpp::integer_type>(it_dc->second);
                newDownload.loaded_size_ = loadsize;
            }
            it_dc = dict_c.find("DownloadEntryProgressTotalToLoad");
            if(it_dc != dict_c.end())
            {
                int32 filesize = boost::any_cast<plistcpp::integer_type>(it_dc->second);
               newDownload.file_size_ =filesize;
            }
            it_dc = dict_c.find("DownloadEntryDateAddedKey");
            if(it_dc != dict_c.end())
            {
                plistcpp::date_type dateTime = boost::any_cast<plistcpp::date_type>(it_dc->second);
                time_t tdt = dateTime.timeAsEpoch();
                newDownload.start_time_ = CPTime::UTC2LocalTime(tdt, siutil::MacCommonHelper::GetTimezoneRegion());
            }
            it_dc = dict_c.find("DownloadEntryDateFinishedKey");
            if(it_dc != dict_c.end())
            {
                plistcpp::date_type dateTime = boost::any_cast<plistcpp::date_type>(it_dc->second);
                time_t tdt = dateTime.timeAsEpoch();
                newDownload.finish_time_ =CPTime::UTC2LocalTime(tdt, siutil::MacCommonHelper::GetTimezoneRegion());
            }

            downloads.push_back(newDownload);
        }

        return true;
    }

    bool CPSafariParser::ParseCache(std::wstring cacheFile, BrowserCaches& caches)
    {
        std::string strFile = CPCodeChanger::UnicodeToUTF8(cacheFile.c_str());
        sqlite3 * db = NULL;
        if(sqlite3_open(strFile.c_str(), &db) != SQLITE_OK)
        {
            return false;
        }

        sqlite3_stmt * pStmt = NULL;

        std::string query = "select request_key, time_stamp from cfurl_cache_response";
        int iRet = sqlite3_prepare(db, query.c_str(), -1, &pStmt, NULL);
        if (SQLITE_OK != iRet)
        {
            return false;
        }
        while(SQLITE_ROW == sqlite3_step(pStmt))
        {
            BrowserCache safariCache;
            safariCache.url_ = CPCodeChanger::UTF8ToUnicode((char *)sqlite3_column_text(pStmt, 0));
            time_t tvt = TimestrToTimet(sqlite3_column_text(pStmt, 1));
            safariCache.visit_time_ = CPTime::UTC2LocalTime(tvt, siutil::MacCommonHelper::GetTimezoneRegion());
            caches.push_back(safariCache);
        }

        sqlite3_finalize(pStmt);
        sqlite3_close(db);
        return true;
    }

    time_t CPSafariParser::TimestrToTimet(const unsigned char* strTime)
    {
        if (NULL == strTime)
        {
            return 0;
        }
        struct tm tm;
        strptime((const char *)strTime, "%Y-%m-%d %H:%M:%S", &tm);
        return timegm(&tm);
    }

    bool CPSafariParser::ParseBookmark(std::wstring bookmarkFile, BookMarkPtr bookmark)
    {
        std::string strFilePath = CPCodeChanger::UnicodeToUTF8(bookmarkFile.c_str());
        plistcpp::dictionary_type dict;
        try
        {
            plistcpp::CPlistCpp::ReadPlist(strFilePath.c_str(), dict);
        }
        catch(...)
        {
            return false;
        }

        BookmarkParseRecursive(dict, bookmark);
        return true;
    }

    void CPSafariParser::BookmarkParseRecursive(plistcpp::dictionary_type& dict, BookMarkPtr bookmark)
    {
        plistcpp::dictionary_type::iterator it_d = dict.find("Title");
        if(it_d != dict.end())
        {
            std::string dirTitle = boost::any_cast<std::string>(it_d->second);
            std::wstring wtitle = CPCodeChanger::UTF8ToUnicode(dirTitle.c_str());
            bookmark->SetDir(true);
            bookmark->SetTitle(wtitle);
        }

        it_d = dict.find("WebBookmarkType");
        if(it_d == dict.end())
        {
            return;
        }
        std::string type = boost::any_cast<std::string>(it_d->second);
        if("WebBookmarkTypeList" == type)
        {
            it_d = dict.find("Children");
            if(it_d == dict.end())
            {
                return;
            }
            plistcpp::array_type arr = boost::any_cast<plistcpp::array_type>(it_d->second);
            plistcpp::array_type::iterator it_a = arr.begin();
            for(; it_a != arr.end(); ++it_a)
            {
                plistcpp::dictionary_type subdict = boost::any_cast<plistcpp::dictionary_type>(*it_a);
                BookmarkParseRecursive(subdict, bookmark);
            }
        }
        else if("WebBookmarkTypeLeaf" == type)
        {
            it_d = dict.find("URIDictionary");
            if(it_d == dict.end())
            {
                return;
            }
            plistcpp::dictionary_type titledict = boost::any_cast<plistcpp::dictionary_type>(it_d->second);
            plistcpp::dictionary_type::iterator it_t = titledict.find("title");
            if(it_t == titledict.end())
            {
                return;
            }
            std::string urlTitle = boost::any_cast<std::string>(it_t->second);

            it_d = dict.find("URLString");
            if(it_d == dict.end())
            {
                return;
            }
            std::string url = boost::any_cast<std::string>(it_d->second);

            BookMarkPtr subBookmark(new CPBookmark);
            subBookmark->SetDir(false);
            subBookmark->SetTitle(CPCodeChanger::UTF8ToUnicode(urlTitle.c_str()));
            subBookmark->SetUrl(CPCodeChanger::UTF8ToUnicode(url.c_str()));
            bookmark->InsertSubItem(subBookmark);
          }
    }

    bool CPSafariParser::ParseCookie(std::wstring cookieFile, BrowserCookies& cookies)
    {
        std::string strFile = CPCodeChanger::UnicodeToUTF8(cookieFile.c_str());
        std::ifstream iFile(strFile.c_str(), std::ios::binary);
        if(iFile.fail())
        {
            return false;
        }

        uint64 FileSize = boost::filesystem::file_size(cookieFile);
        if (FileSize > 30*1024*1024)
        {
            return false;
        }

        byte* buf = new byte[(size_t)FileSize];
        iFile.read((char*)buf, FileSize);
        if (FileSize != (uint64)iFile.tellg())
        {
            return false;
        }

        SAFARI_COOKIES_HEADER* header = (SAFARI_COOKIES_HEADER*)buf;
        if (memcmp(header->Magic, SAFARI_COOKIE_MAGIC, 4))
        {
            return false;
        }
        uint    PageCount = SwapByte(header->PageCount);
        if (PageCount <= 0)
        {
            return false;
        }
        uint HeadSize = PageCount * 4 + sizeof(SAFARI_COOKIES_HEADER);
        if (HeadSize > FileSize)
        {
            return false;
        }

        std::vector<SAFARI_COOKIE_PAGE> CookiePages; //! cookies
        uint PageOff = HeadSize;
        for (uint i = 0;i != PageCount;i++)
        {
            uint PageSize = SwapByte(*(uint*)(buf + sizeof(SAFARI_COOKIES_HEADER) + i*4));
            SAFARI_COOKIE_PAGE tmp;
            tmp.PageOffset = PageOff;
            tmp.PageSize = PageSize;
            CookiePages.push_back(tmp);
            PageOff += PageSize;
        }

        //! ��ÿһ��cookiesҳ���з���
        for (uint i = 0;i<CookiePages.size();i++)
        {
            uint PageOffset = CookiePages[i].PageOffset;
            uint PageSize = CookiePages[i].PageSize;
            if (PageSize < sizeof(SAFARI_COOKIE_PAGE_HEADER))
            {
                continue;
            }
            SAFARI_COOKIE_PAGE_HEADER* pageHead = (SAFARI_COOKIE_PAGE_HEADER*)(buf + PageOffset);
            if (pageHead->HeadSign != SAFARI_COOKIE_PAGE_HEADER_SIGN)
            {
                continue;
            }
            uint RecordCount = pageHead->RecordCount;
            byte* PageBuf = buf + PageOffset;

            if (sizeof(SAFARI_COOKIE_PAGE_HEADER)+ RecordCount*sizeof(uint) + sizeof(SAFARI_COOKIE_PAGE_HEADER_TAIL) > PageSize)
            {
                break;
            }

            //!��ҳ��ÿһ��cookie ��¼���з���
            for (uint j=0;j<RecordCount;j++)
            {
                uint RecordOffset =*(uint*)(PageBuf + sizeof(SAFARI_COOKIE_PAGE_HEADER)+ j*sizeof(uint));
                if (RecordOffset > PageSize)
                {
                    break;
                }

                if (*(uint*)(PageBuf + RecordOffset) > PageSize) //!��¼��С����ҳ��С��������
                {
                    break;
                }

                SAFARI_COOKIE_RECORD* record = (SAFARI_COOKIE_RECORD*)(PageBuf + RecordOffset);

                BrowserCookie newCookie;

                //!Safari Cookiesʹ�õ���CF Absolute Time (Mac Absolute Time)��
                //! ��ʱ��ʹ��double��ʾ�����������ֱ�ʾ��2001��1��1��0:0:0 UTC��ʼ�����ڵ�������
                //!����ʱ��
                time_t expTime = MAC_BASETIME + (uint64) record->expiry;
                newCookie.expireTime_ = CPTime::UTC2LocalTime(expTime, siutil::MacCommonHelper::GetTimezoneRegion());

                //! ��������ʱ��
                time_t createTime = MAC_BASETIME + (uint64)record->LastAccess;
                newCookie.createdTime_ = CPTime::UTC2LocalTime(createTime, siutil::MacCommonHelper::GetTimezoneRegion());
                //! ����
                std::wstring CookieName = CPCodeChanger::UTF8ToUnicode((const char*)(PageBuf + RecordOffset + record->NameOffset));
                //! Url
                std::wstring strUrl = CPCodeChanger::UTF8ToUnicode((const char*)(PageBuf + RecordOffset + record->UrlOffset));
                //! Path
                std::wstring strpath = CPCodeChanger::UTF8ToUnicode((const char*)(PageBuf + RecordOffset + record->PathOffset));
                //!cookieֵ
                std::wstring strValue = CPCodeChanger::UTF8ToUnicode((const char*)(PageBuf + RecordOffset + record->ValueOffset));

                newCookie.name_ = CookieName;
                newCookie.value_  = strValue;
                newCookie.path_ = strpath;
                newCookie.domainUrl_ = strUrl;
     //           pCookie.PosInSrc   = PageOffset + RecordOffset;
     //           pCookie.SizeInSrc  = PageSize;
                cookies.push_back(newCookie);
            }
        }
        delete [] buf;
        return true;
    }

    bool CPSafariParser::GetHistory(CPRecord *rootRecord)
    {
        //MAC10.8, Safari 6.0��/Users/�û���/Library/Safari/History.plist
        //MAC10.10, Safari 8.0��/Users/�û���/Library/Safari/History.db(����History.db-wal�ļ�)

        BrowserHistorys historys;
        //����plist�ļ�
        std::vector<PSIHANDLE> plistHandles;
        CPHandleExport fileExport;
        siutil::MacCommonHelper::GetHandlesByUserRawPath(L"Library/Safari", L"History.plist", plistHandles);
        for(std::vector<PSIHANDLE>::iterator it_h = plistHandles.begin(); it_h != plistHandles.end(); ++it_h)
        {
            std::wstring plistPath = fileExport.Export(*it_h);
            if(!ParsePlistHistory(plistPath, historys)) {
                return false;
            }
        }

        siutil::CPMacFileExport mfx(L"Library/Safari/", L"History.db");
        std::vector<std::wstring> outDbPaths;
        mfx.GetExportFiles(outDbPaths);
        for(std::vector<std::wstring>::iterator it = outDbPaths.begin(); it != outDbPaths.end(); ++it){
            ParseDbHistory(*it, historys);
        }
        BrowserHistorys::iterator it_r = historys.begin();
        for(size_t i = 0; it_r != historys.end(); ++it_r, ++i)
        {
           it_r->AddToTable(rootRecord);
        }
        return true;
    }

    bool CPSafariParser::GetDownload(CPRecord *rootRecord)
    {
        //Safari 6.0  Safari 8.0/Users//Library/Safari/Downloads.plist

        std::vector<PSIHANDLE> plistHandles;
        siutil::MacCommonHelper::GetHandlesByUserRawPath(L"Library/Safari", L"Downloads.plist", plistHandles);
        BrowserDownloads downloads;
        CPHandleExport fileExport;
        for(std::vector<PSIHANDLE>::iterator it_h = plistHandles.begin(); it_h != plistHandles.end(); ++it_h) {
            std::wstring plistPath = fileExport.Export(*it_h);
            ParseDownload(plistPath, downloads);
        }
        BrowserDownloads::iterator it_r = downloads.begin();
        for(size_t i = 0; it_r != downloads.end(); ++it_r, ++i)
        {
            it_r->AddToTable(rootRecord);
        }
        return true;

    }

    bool CPSafariParser::GetCache(CPRecord *rootRecord)
    {
        //Users/�û���/Library/Caches/com.apple.Safari/Cache.db������Cache.db-shm,Cache.db-wal��
        //Users/�û���/Library/Caches/com.apple.Safari/fsCachedData���ǻ����ļ�

        BrowserCaches caches;

        std::vector<std::wstring> dbPaths;
        siutil::CPMacFileExport cmfe(L"/Library/Caches/com.apple.Safari/", L"Cache.db");
        cmfe.GetExportFiles(dbPaths);

        for(std::vector<std::wstring>::iterator it = dbPaths.begin(); it != dbPaths.end(); ++it)
        {
            ParseCache(*it, caches);
        }

       // size_t cacheCount = caches.size();
        BrowserCaches::iterator it_r = caches.begin();
        for(size_t i = 0; it_r != caches.end(); ++it_r, ++i)
        {
            it_r->AddToTable(rootRecord);

        }
        return true;
    }

    bool CPSafariParser::GetBookmark(CPRecord *rootRecord)
    {
       // /Users/�û���/Library/Safari/Bookmarks.plist : ���������е���ǩͬ�����ұ�����Ŀ¼�ṹ
       // 10.8��10.10��/Users/�û���/Library/Caches/Metadata/Safari/Bookmarks/*.webbookmark��
        //�������ӵ���ǩ�����������������Щɾ����

        std::vector<PSIHANDLE> plistHandles;
        CPHandleExport fileExport;
        siutil::MacCommonHelper::GetHandlesByUserRawPath(L"Library/Safari/", L"Bookmarks.plist", plistHandles);

        BrowserBookmarks bookmarks;
        for(std::vector<PSIHANDLE>::iterator it_h = plistHandles.begin(); it_h != plistHandles.end(); ++it_h)
        {      
            std::wstring plistPath = fileExport.Export(*it_h);
            BookMarkPtr oneBookMark = boost::make_shared<CPBookmark>();
            ParseBookmark(plistPath, oneBookMark);
            BrowserBookmarks tempBookmarks;
            oneBookMark->GetBookmarks(tempBookmarks);
            bookmarks.insert(bookmarks.end(), tempBookmarks.begin(), tempBookmarks.end());
        }
        BrowserBookmarks::iterator it_r = bookmarks.begin();
        for(size_t i = 0; it_r != bookmarks.end(); ++it_r, ++i)
        {
            it_r->AddToTable(rootRecord);

        }
        return true;
    }

    bool CPSafariParser::GetCookie(CPRecord *rootRecord)
    {
        //10.8��Safari 6.0��/Users/�û���/Library/Cookies/com.apple.Safari.SafeBrowsing.binarycookies
        //10.10��Safari 8.0��/Users/�û��� /Library/Cookies/Cookies.binarycookies

        std::vector<PSIHANDLE> binaryHandles;
        std::vector<PSIHANDLE> tempHandles1;
        std::vector<PSIHANDLE> tempHandles2;
        siutil::MacCommonHelper::GetHandlesByUserRawPath(L"Library/Cookies/", L"com.apple.Safari.SafeBrowsing.binarycookies", tempHandles1);
        siutil::MacCommonHelper::GetHandlesByUserRawPath(L"Library/Cookies/", L"Cookies.binarycookies", tempHandles2);
        binaryHandles.insert(binaryHandles.end(), tempHandles1.begin(), tempHandles1.end());
        binaryHandles.insert(binaryHandles.end(), tempHandles2.begin(), tempHandles2.end());

        BrowserCookies cookies;
        for(std::vector<PSIHANDLE>::iterator it_h = binaryHandles.begin(); it_h != binaryHandles.end(); ++it_h)
        {
            CPHandleExport fileExport;
            std::wstring cookiePath = fileExport.Export(*it_h);
            ParseCookie(cookiePath, cookies);
        }

        BrowserCookies::iterator it_r = cookies.begin();
        for(size_t i = 0; it_r != cookies.end(); ++it_r, ++i)
        {
            it_r->AddToTable(rootRecord);

        }
        return true;
    }
}
