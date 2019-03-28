#ifndef SIBROWSER_BROWSERHELPER_H
#define SIBROWSER_BROWSERHELPER_H

#include "common.h"
#include "Task/TaskId.h"

namespace task {
    class TaskDelegate;
    class TaskStatusCtrl;
}

namespace SIBrowser
{
    class SIBROWSER_API BrowserHelper
    {
    public:
        BrowserHelper();
        ~BrowserHelper();

        static task::CallBack GetCallBackByTaskId(task::TaskDefId id);
    };
}

#endif // SIBROWSER_BROWSERHELPER_H
