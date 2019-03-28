#pragma once
#include "PBrowserInformation.h"
namespace macbasic
{
class CPBookmark;
typedef boost::shared_ptr<CPBookmark> BookMarkPtr;
typedef std::vector<BookMarkPtr> BookMarkPtrs;

class CPBookmark
{
public:
    CPBookmark();
    ~CPBookmark();

    void SetDir(bool bd);
    bool IsDir();
    int  SubItemCount();
    void InsertSubItem(BookMarkPtr bookmark);
    void GetSubItems(BookMarkPtrs& subItems);
    void SetTitle(std::wstring title);
    std::wstring GetTitle();
    void SetUrl(std::wstring url);
    std::wstring GetUrl();
    //不分结构，只把网页提取出来
    void GetBookmarks(BrowserBookmarks& bookmarks);

private:
    std::wstring title_; // 书签(文件夹)标题
    std::wstring url_;

    bool isDir_; //是否文件夹
    BookMarkPtrs subItems_; //书签子条目
};
}
