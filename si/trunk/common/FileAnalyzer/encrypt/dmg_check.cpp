#include "stdafx.h"

namespace pfile {

bool IsEncrypted_dmg(PSIHANDLE pFile)
{
    uint64 fileSize = CFileSystem::GetItemSize(pFile);
    const uint64 MAGIC_SIZE = 8;
    if(fileSize < MAGIC_SIZE)
    {
        return false;
    }
    const std::string magic = "encrcdsa";
    byte buf[MAGIC_SIZE] = {0};
    CFileSystem::ReadFile(pFile, 0, 8, buf);
    int ret = memcmp(magic.c_str(), buf, MAGIC_SIZE);
    if(ret == 0) {
        return true;
    }
    else {
        return false;
    }
}

}
