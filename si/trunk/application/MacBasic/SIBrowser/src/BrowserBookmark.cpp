#include "stdafx.h"
#include "SIBrowser/include/SIBrowser/BrowserBookmark.h"

namespace macbasic
{
    CBookmark::CBookmark()
        : isDir_(false)
    {

    }

    CBookmark::~CBookmark()
    {

    }

    void CBookmark::SetDir(bool bd)
    {
        isDir_ = bd;
    }

    bool CBookmark::IsDir()
    {
        return isDir_;
    }

    int CBookmark::SubItemCount()
    {
        return subItems_.size();
    }

    void CBookmark::InsertSubItem(BookMarkPtr bookmark)
    {
        subItems_.push_back(bookmark);
    }

    void CBookmark::GetSubItems(BookMarkPtrs& subItems)
    {
        subItems = subItems_;
    }

    void CBookmark::SetTitle(std::wstring title)
    {
        title_ = title;
    }

    std::wstring CBookmark::GetTitle()
    {
        return title_;
    }

    void CBookmark::SetUrl(std::wstring url)
    {
        url_ = url;
    }

    std::wstring CBookmark::GetUrl()
    {
        return url_;
    }

    void CBookmark::GetBookmarks(BrowserBookmarks& bookmarks)
    {
        if(this->IsDir())
        {
            for(BookMarkPtrs::iterator it = subItems_.begin(); it != subItems_.end(); ++it)
            {
                (*it)->GetBookmarks(bookmarks);
            }
        }
        else {
            if(title_.empty() && url_.empty())
            {
                return;
            }
            BrowserBookmark oneBookmark;
            oneBookmark.title_ = title_;
            oneBookmark.url_ = url_;
            bookmarks.push_back(oneBookmark);
        }
    }
}
