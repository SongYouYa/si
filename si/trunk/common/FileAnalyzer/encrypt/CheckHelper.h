#pragma once

namespace pfile {

class CheckHelper
{
public:
    CheckHelper();
    static bool CheckOLEHeader(PSIHANDLE pFile);
    static bool IsEncrypted(PSIHANDLE pFile);
};

}