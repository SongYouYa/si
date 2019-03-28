#ifndef MAC_MACIMHELPER_H
#define MAC_MACIMHELPER_H

namespace macim {

    class  MacIMHelper
    {
    public:
        MacIMHelper();
        //===========================================================
        // brief :		获取即时通信记录回调函数
        // Returns:		task::CallBack	获取的回调函数
        // Parameter:	id	回调函数对应的ID信息
        // Author:    	jiaowei
        // DATE:      	2015.8.4 14:01:11
        //===========================================================
        static task::CallBack GetCallbackByTaskId(task::TaskDefId id);
    };

}

#endif // MACIMHELPER_H
