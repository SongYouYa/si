#include "stdafx.h"
#include "PBrowserBookmark.h"

namespace macbasic
{
    CPBookmark::CPBookmark()
        : isDir_(false)
    {

    }

    CPBookmark::~CPBookmark()
    {

    }

    void CPBookmark::SetDir(bool bd)
    {
        isDir_ = bd;
    }

    bool CPBookmark::IsDir()
    {
        return isDir_;
    }

    int CPBookmark::SubItemCount()
    {
        return subItems_.size();
    }

    void CPBookmark::InsertSubItem(BookMarkPtr bookmark)
    {
        subItems_.push_back(bookmark);
    }

    void CPBookmark::GetSubItems(BookMarkPtrs& subItems)
    {
        subItems = subItems_;
    }

    void CPBookmark::SetTitle(std::wstring title)
    {
        title_ = title;
    }

    std::wstring CPBookmark::GetTitle()
    {
        return title_;
    }

    void CPBookmark::SetUrl(std::wstring url)
    {
        url_ = url;
    }

    std::wstring CPBookmark::GetUrl()
    {
        return url_;
    }

    void CPBookmark::GetBookmarks(BrowserBookmarks& bookmarks)
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
