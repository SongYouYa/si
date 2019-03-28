#ifndef SIBROWSER_BROWSERBOOKMARK_H
#define SIBROWSER_BROWSERBOOKMARK_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "BrowserInformation.h"

namespace macbasic
{
class CBookmark;
typedef boost::shared_ptr<CBookmark> BookMarkPtr;
typedef std::vector<BookMarkPtr> BookMarkPtrs;

class CBookmark
{
public:
    CBookmark();
    ~CBookmark();

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

#endif // SIBROWSER_BROWSERBOOKMARK_H
