#pragma once
#include "siio/PDeviceWriter.h"

namespace devtask {

class CPSiDeviceTaskCommon
{
public:
    static bool onOpenWriterFailed(const std::wstring& diskPath,
                                   siio::CPDeviceWriterPtr writer,
                                   CPTaskStatusCtrl* statusCtrl);
};

}
