#pragma once
#include "siutility/globals.h"
class CPTaskStatusCtrl;

namespace siutil
 {
    class SI_UTILITY_API CPSiFileUtils
    {
    public:
        CPSiFileUtils();

        static bool exportFile(const std::wstring& sorcePath, const std::wstring& targetPath);
        static bool exportFile(PSIHANDLE handle, const std::wstring& targetPath,
                               std::string* md5Value = nullptr, CPTaskStatusCtrl* statusCtrl = nullptr,
                               const uint64* pTotalSize = nullptr, uint64* pCurSize = nullptr);
         static std::wstring calcHashValue(PSIHANDLE fileHandle);
    };
}
