#pragma once
#include "siutility/PFileExport.h"

namespace siutil
{
    class  CPMacFileExport
    {
    public:
        CPMacFileExport(const std::wstring userRawRootPath, const std::wstring fileName);
        ~CPMacFileExport();

        void GetExportFiles(std::vector<std::wstring>& exportFiles);
    private:
        std::wstring userRawRootPath_;
        std::wstring fileName_;
        std::vector<std::wstring> exportedFiles_;
        siutil::CFileExportPtrs cfxptrs_;
    };
}
