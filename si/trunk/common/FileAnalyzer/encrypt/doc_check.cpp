#include "stdafx.h"

namespace pfile {

#pragma pack(push,1)
struct FIB
{
    unsigned short      magic;          // Magic number
    unsigned short      fibVersion;     // fib version
    unsigned short      productVersion; // Product version
    unsigned short      langStamp;      // language stamp
    unsigned short      next;
    unsigned short      flags;          // all file flags
    unsigned short      nFibBack;
    unsigned short		lKey;
    // ...
};
#pragma pack(pop)

bool IsEncrypted_doc(PSIHANDLE pFile)
{
    if (NULL == pFile) {
        return false;
    }
    if (CFileSystem::GetItemSize(pFile) < 1024) {
        return false;
    }

    FIB docFib;
    const int nCompoundHeadSize = 0x200;
    CFileSystem::ReadFile(pFile, nCompoundHeadSize, sizeof(FIB), (byte*)&docFib);

#if 1
    //mac
    bool be = (docFib.flags & 0x100);
#else
    bool be = ((docFib.flags & 0x0080) != 0) && ((docFib.flags & 0x0001) == 0) && docFib.lKey > 0;
#endif

    return be;
}

bool IsEncrypted_docx(PSIHANDLE pFile)
{
    std::vector<std::string> signs;
#if 1
    //mac
    signs.push_back("EncryptedPackage");
    signs.push_back("Microsoft.Container.DataSpaces");
#endif
    bool be = CommonCheck::IsContainSigns(pFile, signs, 0x4000);

    return be;
}

}
