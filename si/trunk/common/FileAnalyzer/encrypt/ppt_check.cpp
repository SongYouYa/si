#include "stdafx.h"


namespace pfile {

bool IsEncrypted_pptx(PSIHANDLE pFile)
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

bool IsEncrypted_ppt(PSIHANDLE pFile)
{
    std::vector<std::string> signs;
#if 1
    //MAC
    signs.push_back("PowerPoint Document");
    signs.push_back("EncryptedSummary");
#endif
    bool be = CommonCheck::IsContainSigns(pFile, signs, 0x4000);

    return be;
}
}
