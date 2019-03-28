#ifndef _SI_MAC_SYSTEMINFOHELPER_H
#define _SI_MAC_SYSTEMINFOHELPER_H

#include "Common.h"
#include "Task/TaskId.h"


namespace systeminfo {
    class SYSTEMINFO_API SystemInfoHelper
    {
    public:
        SystemInfoHelper();
        ~SystemInfoHelper();

        //===========================================================
        // brief :		获取系统信息回调函数
        // Returns:		task::CallBack	获取的回调函数
        // Parameter:	id	回调函数对应的ID信息
        // Author:    	zxj
        // DATE:      	2015.8.20 14:01:11
        //===========================================================
        static task::CallBack GetCallbackByTaskId(task::TaskDefId id);

		static std::wstring GetSystemRootPath(); //系统根目录
    };
}


#endif // SYSTEMINFOHELPER_H
