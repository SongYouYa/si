#include "SIBrowser/include/SIBrowser/BrowserHelper.h"
#include "SIBrowser/include/SIBrowser/BrowserInformation.h"
#include "SIBrowser/include/SIBrowser/SafariParser.h"
#include "SIBrowser/include/SIBrowser/ChromeParser.h"
//#include "FileSystem/LoadDeviceThread.h"
#include <boost/bind.hpp>

namespace SIBrowser
{
    BrowserHelper::BrowserHelper()
    {

    }

    BrowserHelper::~BrowserHelper()
    {

    }

    task::CallBack BrowserHelper::GetCallBackByTaskId( task::TaskDefId id )
    {
        switch (id)
        {
        case task::TaskNone:
            break;
#ifdef SI_MAC
        case task::TaskSafariHistory:
        {
            CSafariParser safariParser;
            return boost::bind(&CSafariParser::GetHistory, safariParser, _1, _2);
        }
        case task::TaskSafariDownload:
        {
            CSafariParser safariParser;
            return boost::bind(&CSafariParser::GetDownload, safariParser, _1, _2);
        }
        case task::TaskSafariCache:
        {
            CSafariParser safariParser;
            return boost::bind(&CSafariParser::GetCache, safariParser, _1, _2);
        }
        case task::TaskSafariBookmark:
        {
            CSafariParser safariParser;
            return boost::bind(&CSafariParser::GetBookmark, safariParser, _1, _2);
        }
        case task::TaskSafariCookie:
        {
            CSafariParser safariParser;
            return boost::bind(&CSafariParser::GetCookie, safariParser, _1, _2);
        }
        case task::TaskChromeHistory:
        {
            CChromeParser chromeParser;
            return boost::bind(&CChromeParser::GetHistory, chromeParser, _1, _2);
        }
        case task::TaskChromeDownload:
        {
            CChromeParser chromeParser;
            return boost::bind(&CChromeParser::GetDownload, chromeParser, _1, _2);
        }
        case task::TaskChromeCache:
        {
            CChromeParser chromeParser;
            return boost::bind(&CChromeParser::GetCache, chromeParser, _1, _2);
        }
        case task::TaskChromeBookmark:
        {
            CChromeParser chromeParser;
            return boost::bind(&CChromeParser::GetBookmark, chromeParser, _1, _2);
        }
        case task::TaskChromeCookie:
        {
            CChromeParser chromeParser;
            return boost::bind(&CChromeParser::GetCookie, chromeParser, _1, _2);
        }
        case task::TaskChromeForm:
        {
            CChromeParser chromeParser;
            return boost::bind(&CChromeParser::GetForm, chromeParser, _1, _2);
        }
#endif
        default:
            break;
        }
        return NULL;
    }
}
