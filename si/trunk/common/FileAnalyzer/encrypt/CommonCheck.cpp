#include "stdafx.h"
#include "CommonCheck.h"


namespace pfile {

CommonCheck::CommonCheck()
{

}

bool CommonCheck::IsContainSigns(PSIHANDLE pFile, const std::vector<std::string>& signs, int headerSize)
{
    if (NULL == pFile)
    {
        return false;
    }

    uint64 ReadSize = headerSize;
    uint64 FileSize = CFileSystem::GetItemSize(pFile);
    ReadSize = pmin(FileSize,ReadSize);
    byte* buf = new byte[(uint)ReadSize];
    CFileSystem::ReadFile(pFile, 0, ReadSize, buf);

    uint64 offset = 0;
    size_t mark = 0;
    for(auto it = signs.begin(); it != signs.end(); ++it)
    {
        std::string sign = *it;
        std::string wsign = CharToChar16(sign);

        while(offset + wsign.size() < ReadSize)
        {
            if (memcmp((char*)(buf+offset), wsign.c_str(), wsign.size()) == 0)
            {
                ++mark;
                break;
            }
            ++offset;
        }
    }
    delete [] buf;

    return mark == signs.size();
}

std::string CommonCheck::CharToChar16(const std::string& str)
{
    std::string tempstr;
    for(size_t i = 0; i < str.size(); ++i)
    {
        tempstr.push_back(str.at(i));
        tempstr.push_back('\0');
    }
    return tempstr;
}

}
