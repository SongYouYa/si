#include "stdafx.h"

namespace pfile {

bool IsEncrypted_xlsx(PSIHANDLE pFile)
{
    std::vector<std::string> signs;
#if 1
    //MAC
    signs.push_back("EncryptedPackage");
    signs.push_back("Microsoft.Container.DataSpaces");
#endif
    bool be = CommonCheck::IsContainSigns(pFile, signs, 0x4000);

    return be;
}

bool IsEncrypted_xls(PSIHANDLE pFile)
{
    return false;
}

}
