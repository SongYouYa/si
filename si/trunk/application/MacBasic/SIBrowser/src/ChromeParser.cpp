#include "stdafx.h"
#include <QDateTime>
#include "SIBrowser/include/SIBrowser/ChromeParser.h"
#include "SIBrowser/include/SIBrowser/ChromeCacheParser.h"
#include "siutility/PropertyTreeHelper.h"
// /Users/*/Library/Application Support/Google/Chrome/Default/
const std::wstring MAC_CHROME_DEFAULT = L"Users/*/Library/Application Support/Google/Chrome/Default/";
const std::wstring MAC_CHROME_CACHE = L"Users/*/Library/Caches/Google/Chrome/Default/Cache/";

using namespace filesystem;
using namespace boost::property_tree;

namespace  macbasic
{

    CChromeParser::CChromeParser()
    {

    }

    CChromeParser::~CChromeParser()
    {

    }

    bool CChromeParser::ParseHistory(std::wstring historyFile, BrowserHistorys& historys)
    {
        std::string strFile = CPCodeChanger::UnicodeToUTF8(historyFile.c_str());
        sqlite3* db = NULL;

        if(sqlite3_open(strFile.c_str(), &db) != SQLITE_OK)
        {
            return false;
        }
        std::string query = "select u.title, u.url, v.visit_time from urls as u, visits as v where v.url = u.id order by v.visit_time desc";
        sqlite3_stmt *stmt = NULL;
        if(sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) != SQLITE_OK)
        {
            return false;
        }
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            BrowserHistory newHistory;
            newHistory.title_ = CPCodeChanger::UTF8ToUnicode((char*)sqlite3_column_text(stmt, 0));
            newHistory.url_ = CPCodeChanger::UTF8ToUnicode((char*)sqlite3_column_text(stmt, 1));     
            int64 visitTime = sqlite3_column_int64(stmt, 2);
            CPTime utcpt(CPTime::Int64ToFileTime(visitTime*10));
           // CPTime:
            newHistory.visit_time_ = CPTime::UTC2LocalTime(utcpt, siutility::MacCommonHelper::GetTimezoneRegion());
            newHistory.visit_count_ = 1;
            historys.push_back(newHistory);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    //download
    bool CChromeParser::ParseDownload(std::wstring downloadFile, BrowserDownloads& downloads)
    {
        std::string strFile = CPCodeChanger::UnicodeToUTF8(downloadFile.c_str());
        sqlite3* db = NULL;

        if(sqlite3_open(strFile.c_str(), &db) != SQLITE_OK)
        {
            return false;
        }

        std::string query = "select d.target_path, u.url, d.total_bytes, d.received_bytes, d.start_time, d.end_time from downloads as d, \
                (select * from (select * from downloads_url_chains order by id, chain_index) group by id) as u \
                where d.id = u.id order by d.start_time desc";
        sqlite3_stmt *stmt = NULL;
        if(sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) != SQLITE_OK)
        {
            return false;
        }
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            BrowserDownload newDownload;
            newDownload.savePath_ = CPCodeChanger::UTF8ToUnicode((char*)sqlite3_column_text(stmt, 0));
            newDownload.url_ = CPCodeChanger::UTF8ToUnicode((char*)sqlite3_column_text(stmt, 1));
            uint64 filesize = sqlite3_column_int64(stmt, 2);
            newDownload.file_size_ = siutility::AutoUnitConvert::BytesToUintString(filesize);
            uint64 loadsize = sqlite3_column_int64(stmt, 3);
            newDownload.loaded_size_ = siutility::AutoUnitConvert::BytesToUintString(loadsize);
            int64 visitTime = sqlite3_column_int64(stmt, 4);
            CPTime utcpt(CPTime::Int64ToFileTime(visitTime*10));
            newDownload.start_time_ = CPTime::UTC2LocalTime(utcpt, siutility::MacCommonHelper::GetTimezoneRegion());
            int64 endTime = sqlite3_column_int64(stmt, 5);
            if(endTime != 0)
            {
                CPTime utcpt(CPTime::Int64ToFileTime(endTime*10));
                newDownload.finish_time_ = CPTime::UTC2LocalTime(utcpt, siutility::MacCommonHelper::GetTimezoneRegion());
            }
            downloads.push_back(newDownload);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    bool CChromeParser::ParseBookmark(std::wstring bookmarkFile, BookMarkPtr bookmark)
    {
        wptree pt;
        if(!siutility::CPropertyTreeHelper::LoadJson(bookmarkFile, pt))
        {
            return false;
        }
        wptree& pt_c = pt.get_child(L"roots.bookmark_bar");
        BookmarkRecursive(pt_c, bookmark);
        return true;
    }
    void CChromeParser::BookmarkRecursive(boost::property_tree::wptree& pt, BookMarkPtr bookmark)
    {
        std::wstring strname = pt.get<std::wstring>(L"name");
        bookmark->SetTitle(strname);

        std::wstring strType = pt.get<std::wstring>(L"type");
        if(strType == L"folder")
        {
            bookmark->SetDir(true);
            wptree& pt_c = pt.get_child(L"children");
           int count = pt_c.size();
            wptree::iterator it_pt_c = pt_c.begin();
            for(int i = 0; i < count; ++i, ++it_pt_c)
            {
                BookMarkPtr subBookmark(new CBookmark);
                wptree& pt_c2 = it_pt_c->second;
                BookmarkRecursive(pt_c2, subBookmark);
                bookmark->InsertSubItem(subBookmark);
            }
        }
        else if(strType == L"url")
        {
            bookmark->SetDir(false);
            std::wstring strurl = pt.get<std::wstring>(L"url");
            bookmark->SetUrl(strurl);
        }
    }

    bool CChromeParser::ParseCookie(std::wstring cookieFile, BrowserCookies& cookies)
    {
        std::string strFile = CPCodeChanger::UnicodeToUTF8(cookieFile.c_str());

        sqlite3* db = NULL;

        if(sqlite3_open(strFile.c_str(), &db) != SQLITE_OK)
        {
            return false;
        }

        std::string query = "select name, value, host_key, path, creation_utc, expires_utc from cookies";
        sqlite3_stmt *stmt = NULL;
        if(sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) != SQLITE_OK)
        {
            return false;
        }

        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            BrowserCookie newCookie;
            newCookie.name_ = CPCodeChanger::UTF8ToUnicode((char*)sqlite3_column_text(stmt, 0));
            newCookie.value_ = CPCodeChanger::UTF8ToUnicode((char*)sqlite3_column_text(stmt, 1));
            newCookie.domainUrl_ = CPCodeChanger::UTF8ToUnicode((char*)sqlite3_column_text(stmt, 2));
            newCookie.path_ = CPCodeChanger::UTF8ToUnicode((char*)sqlite3_column_text(stmt, 3));
            int64 createTime = sqlite3_column_int64(stmt, 4);
            CPTime cpt(CPTime::Int64ToFileTime(createTime*10));
            newCookie.createdTime_ = CPTime::UTC2LocalTime(cpt, siutility::MacCommonHelper::GetTimezoneRegion());
            int64 expTime = sqlite3_column_int64(stmt, 5);
            CPTime ept(CPTime::Int64ToFileTime(expTime*10));
            newCookie.expireTime_ = CPTime::UTC2LocalTime(ept, siutility::MacCommonHelper::GetTimezoneRegion());
            cookies.push_back(newCookie);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    bool CChromeParser::ParseForm(std::wstring formFile, BrowserForms& forms)
    {
        std::string strFile = CPCodeChanger::UnicodeToUTF8(formFile.c_str());
        sqlite3* db = NULL;

        if(sqlite3_open(strFile.c_str(), &db) != SQLITE_OK)
        {
            return false;
        }
        //����·�������ж���url��downloads_url_chains::chain_index�����ţ�ȡ����һ��
        std::string query = "select name, value, date_created, date_last_used, count from autofill";
        sqlite3_stmt *stmt = NULL;
        if(sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) != SQLITE_OK)
        {
            return false;
        }
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            BrowserForm newForm;
            newForm.name_ = CPCodeChanger::UTF8ToUnicode((char*)sqlite3_column_text(stmt, 0));
            newForm.value_ = CPCodeChanger::UTF8ToUnicode((char*)sqlite3_column_text(stmt, 1));
            time_t ctt = sqlite3_column_int64(stmt, 2);
            newForm.createdTime_ = CPTime::UTC2LocalTime(ctt, siutility::MacCommonHelper::GetTimezoneRegion());
            time_t utt = sqlite3_column_int64(stmt, 3);
            newForm.lastUsedTime_ = CPTime::UTC2LocalTime(utt, siutility::MacCommonHelper::GetTimezoneRegion());
            newForm.count_ = sqlite3_column_int64(stmt, 4);
            forms.push_back(newForm);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    bool CChromeParser::GetHistory(CPRecord *rootRecord)
    {
        // /Users/�û���/Library/macbasiclication Support/Google/Chrome/Default/History

        std::vector<PSIHANDLE> dbHandles;
        std::wstring historyRawPath = MAC_CHROME_DEFAULT + L"History";
        CFileSystem::GetItemHandlesByRawPath(historyRawPath, dbHandles);

        BrowserHistorys historys;
        for(std::vector<PSIHANDLE>::iterator it_h = dbHandles.begin(); it_h != dbHandles.end(); ++it_h)
        {
            siutility::CHandleExport che;
            std::wstring dbPath = che.Export(*it_h);
            ParseHistory(dbPath, historys);
        }

        CPRecordType* chromeType = CPRecordTypeFactory::get()->getType(L"table.browserhistory");
            CPRecord* osRootRecord = chromeType->newRecord();
            rootRecord->addChild(osRootRecord);
        BrowserHistorys::iterator it_r = historys.begin();
        for(size_t i = 0; it_r != historys.end(); ++it_r, ++i)
        {
           it_r->AddToTable(osRootRecord);
        }
        return true;
    }

    bool CChromeParser::GetDownload(CPRecord *rootRecord)
    {
        // /Users/*/Library/macbasiclication Support/Google/Chrome/Default/History

        std::vector<PSIHANDLE> dbHandles;
        std::wstring downloadRawPath = MAC_CHROME_DEFAULT + L"History";
        CFileSystem::GetItemHandlesByRawPath(downloadRawPath, dbHandles);

        BrowserDownloads downloads;
        for(std::vector<PSIHANDLE>::iterator it_h = dbHandles.begin(); it_h != dbHandles.end(); ++it_h)
        {
            siutility::CHandleExport che;
            std::wstring dbPath = che.Export(*it_h);
            ParseDownload(dbPath, downloads);
        }
        CPRecordType* downloadsType = CPRecordTypeFactory::get()->getType(L"table.browserdownload");
        CPRecord* osDownloadRootRecord = downloadsType->newRecord();
        rootRecord->addChild(osDownloadRootRecord);
        BrowserDownloads::iterator it_r = downloads.begin();
        for(size_t i = 0; it_r != downloads.end(); ++it_r, ++i)
        {
           it_r->AddToTable(osDownloadRootRecord);

        }
        return true;
    }

     bool CChromeParser::GetCache(CPRecord *rootRecord)
     {
         std::vector<PSIHANDLE> indexHandles;
         std::wstring indexRawPath = MAC_CHROME_CACHE + L"index";
         CFileSystem::GetItemHandlesByRawPath(indexRawPath, indexHandles);

         BrowserCaches caches;
         for(std::vector<PSIHANDLE>::iterator it = indexHandles.begin(); it != indexHandles.end(); ++it)
         {
             CPChromeCacheParser chromecachepa;
             chromecachepa.ParseFile(*it);
             BrowserCaches tempcaches;
             chromecachepa.GetCaches(tempcaches);
             caches.insert(caches.end(), tempcaches.begin(), tempcaches.end());
         }

         CPRecordType* cacheTypCPRecord = CPRecordTypeFactory::get()->getType(L"table.browsercache");
         CPRecord* osCacheRootRecord = cacheTypCPRecord->newRecord();
         rootRecord->addChild(osCacheRootRecord);

         BrowserCaches::iterator it_r = caches.begin();
         for(size_t i = 0; it_r != caches.end(); ++it_r, ++i)
         {
            it_r->AddToTable(rootRecord);

         }
         return true;
     }

     bool CChromeParser::GetBookmark(CPRecord *rootRecord)
     {
        // /Users/*/Library/Application Support/Google/Chrome/Default/Bookmarks
        std::vector<PSIHANDLE> bookHandles;
        std::wstring bookRawPath = MAC_CHROME_DEFAULT + L"Bookmarks";
        CFileSystem::GetItemHandlesByRawPath(bookRawPath, bookHandles);

        BrowserBookmarks bookmarks;
        for(std::vector<PSIHANDLE>::iterator it = bookHandles.begin(); it != bookHandles.end(); ++it)
        {
            siutility::CHandleExport che;
            std::wstring bookPath = che.Export(*it);
            BookMarkPtr oneBookMark(new CBookmark);
            if(!ParseBookmark(bookPath, oneBookMark))
            {
                continue;
            }
            BrowserBookmarks tempBookmarks;
            oneBookMark->GetBookmarks(tempBookmarks);
            bookmarks.insert(bookmarks.end(), tempBookmarks.begin(), tempBookmarks.end());
        }

        CPRecordType* bookMarksTypCPRecord = CPRecordTypeFactory::get()->getType(L"table.browserbookmark");
        CPRecord* osBookRootRecord = bookMarksTypCPRecord->newRecord();
        rootRecord->addChild(osBookRootRecord);

        BrowserBookmarks::iterator it_r = bookmarks.begin();
        for(size_t i = 0; it_r != bookmarks.end(); ++it_r, ++i)
        {
           it_r->AddToTable(osBookRootRecord);

        }
        return true;
     }

     bool CChromeParser::GetCookie(CPRecord *rootRecord)
     {
         // /Users/*/Library/macbasiclication Support/Google/Chrome/Default/Cookies

         std::vector<PSIHANDLE> dbHandles;
         std::wstring cookieRawPath = MAC_CHROME_DEFAULT + L"Cookies";
         CFileSystem::GetItemHandlesByRawPath(cookieRawPath, dbHandles);

         BrowserCookies cookies;
         for(std::vector<PSIHANDLE>::iterator it_h = dbHandles.begin(); it_h != dbHandles.end(); ++it_h)
         {
             siutility::CHandleExport che;
             std::wstring plistPath = che.Export(*it_h);
             ParseCookie(plistPath, cookies);
         }
         CPRecordType* cookiesTypCPRecord = CPRecordTypeFactory::get()->getType(L"table.browsercookie");
         CPRecord* osCookRootRecord = cookiesTypCPRecord->newRecord();
         rootRecord->addChild(osCookRootRecord);
         BrowserCookies::iterator it_r = cookies.begin();
         for(size_t i = 0; it_r != cookies.end(); ++it_r, ++i)
         {
            it_r->AddToTable(osCookRootRecord);
         }
         return true;
     }

     bool CChromeParser::GetForm(CPRecord *rootRecord)
     {
         // /Users/*/Library/macbasiclication Support/Google/Chrome/Default/Web Data

         std::vector<PSIHANDLE> dbHandles;
         std::wstring formRawPath = MAC_CHROME_DEFAULT + L"Web Data";
         CFileSystem::GetItemHandlesByRawPath(formRawPath, dbHandles);

         BrowserForms forms;
         for(std::vector<PSIHANDLE>::iterator it_h = dbHandles.begin(); it_h != dbHandles.end(); ++it_h)
         {
             siutility::CHandleExport che;
             std::wstring dbPath = che.Export(*it_h);
             ParseForm(dbPath, forms);
         }
         CPRecordType* formTypCPRecord = CPRecordTypeFactory::get()->getType(L"table.browserform");
         CPRecord* osformRootRecord = formTypCPRecord->newRecord();
         rootRecord->addChild(osformRootRecord);

         BrowserForms::iterator it_r = forms.begin();

         for(size_t i = 0; it_r != forms.end(); ++it_r, ++i)
         {
            it_r->AddToTable(osformRootRecord);

         }
         return true;
     }
}
