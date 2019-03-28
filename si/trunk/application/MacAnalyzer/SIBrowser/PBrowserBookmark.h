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
    //���ֽṹ��ֻ����ҳ��ȡ����
    void GetBookmarks(BrowserBookmarks& bookmarks);

private:
    std::wstring title_; // ��ǩ(�ļ���)����
    std::wstring url_;

    bool isDir_; //�Ƿ��ļ���
    BookMarkPtrs subItems_; //��ǩ����Ŀ
};
}
