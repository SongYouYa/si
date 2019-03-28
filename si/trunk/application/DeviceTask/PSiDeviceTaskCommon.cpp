#include "stdafx.h"
#include "PSiDeviceTaskCommon.h"

#include "sidevice/PDiskHandler.h"

using namespace pdevice;

namespace devtask {

bool CPSiDeviceTaskCommon::onOpenWriterFailed(const std::wstring &diskPath,
                                              siio::CPDeviceWriterPtr writer,
                                              CPTaskStatusCtrl *statusCtrl)
{
#ifdef OS_PAPPLE
    //mac下如果打开磁盘失败，则先umount磁盘，再试下
    if (!CPDiskHandler::setDiskOnline(diskPath, false)
            || !writer->open(diskPath)) {
#endif
        statusCtrl->fail(TASK_ERROR_OPEN_DEVICE);
        PLOG(PLOG_ERROR, L"打开磁盘错误");
        return false;
#ifdef OS_PAPPLE
    }
#endif
    return true;
}

}
