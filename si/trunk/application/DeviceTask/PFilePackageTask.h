#pragma once

namespace devtask {

class CPFilePackageTask
{
public:
    static bool doPackage(const devtask::DeviceImageParam& param,
                          CPTask* task);
};

}
