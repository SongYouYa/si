#pragma once
#include "base/PRecord.h"

namespace macbasic
{
struct CPBrowserHistory
{
    std::wstring        title_;
    std::wstring        url_;
    CPTime     visit_time_; //last visit time
    uint64     visit_count_;

    CPBrowserHistory()
        : visit_count_(0)
    {
    }

    void AddToTable(CPRecord* rootRecord)
    {
        CPRecord* browserInfoRecord = rootRecord->addChild();
        browserInfoRecord->at(L"Title") = title_;
        browserInfoRecord->at(L"Url") = url_;
        browserInfoRecord->at(L"VisitTime") = visit_time_;
        browserInfoRecord->at(L"VisitCount") = visit_count_;

    }
};
typedef std::vector<CPBrowserHistory> BrowserHistorys;

struct CPBrowserDownload
{
    std::wstring    savePath_;
    std::wstring    url_;
    uint64 file_size_;
    uint64  loaded_size_;
    CPTime start_time_;
    CPTime finish_time_;

    void AddToTable(CPRecord* rootRecord)
    {
        CPRecord* browserRecord = rootRecord->addChild();
        browserRecord->at(L"SavePath") = savePath_;
        browserRecord->at(L"Url") = url_;
        browserRecord->at( L"FileSize") = file_size_;
        browserRecord->at(L"LoadedSize") = loaded_size_;
        browserRecord->at(L"StartTime") = start_time_;
        browserRecord->at(L"FinishTime") = finish_time_;
    }
};
typedef std::vector<CPBrowserDownload> BrowserDownloads;

struct BrowserCache
{
    std::wstring      url_;
    CPTime     visit_time_; //��������ʱ��

    BrowserCache()
    {
    }

    void AddToTable(CPRecord* rootRecord)
    {
        CPRecord* browserVisitRecord = rootRecord->addChild();
        browserVisitRecord->at( L"Url") = url_;
        browserVisitRecord->at( L"VisitTime") = visit_time_;
    }
};
typedef std::vector<BrowserCache> BrowserCaches;

struct BrowserBookmark
{
    std::wstring title_;
    std::wstring url_;

    void AddToTable(CPRecord* rootRecord)
    {
        CPRecord* browserRecord = rootRecord->addChild();
        browserRecord->at(L"Title") = title_;
        browserRecord->at(L"Url") = url_;
    }
};
typedef std::vector<BrowserBookmark> BrowserBookmarks;

struct BrowserCookie
{
    std::wstring name_; //����
    std::wstring value_; //����
    std::wstring domainUrl_; //��
    std::wstring path_; //·��
    CPTime createdTime_; //����ʱ��
    CPTime expireTime_; //����ʱ��

    BrowserCookie()
    {
    }

    void AddToTable(CPRecord* rootRecord)
    {
        CPRecord* cookieRecord = rootRecord->addChild();
        cookieRecord->at(L"Name") = name_;
        cookieRecord->at(L"Value") = value_;
        cookieRecord->at(L"DomainUrl") = domainUrl_;
        cookieRecord->at(L"Path") = path_;
        cookieRecord->at(L"ExpireTime") = expireTime_;
        cookieRecord->at(L"CreatedTime") = createdTime_;
    }
};
typedef std::vector<BrowserCookie> BrowserCookies;

struct BrowserForm // ������¼
{
    std::wstring name_; //����
    std::wstring value_; //����
    CPTime createdTime_; //����ʱ��
    CPTime lastUsedTime_; //����ʹ��ʱ��
    int64 count_; //ʹ�ô���

    BrowserForm()
    {
    }

    void AddToTable(CPRecord* rootRecord)
    {
        CPRecord* formRecord = rootRecord->addChild();
        formRecord->at(L"Name") = name_;
        formRecord->at(L"Value") = value_;
        formRecord->at(L"CreatedTime") = createdTime_;
        formRecord->at(L"LastUsedTime") = lastUsedTime_;
        formRecord->at(L"Count") = count_;
    }
};
typedef std::vector<BrowserForm> BrowserForms;

}

