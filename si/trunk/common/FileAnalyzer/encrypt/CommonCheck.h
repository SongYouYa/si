#pragma once

namespace pfile {

class CommonCheck
{
public:
    CommonCheck();

    // 顺序检查文件中是否包含signs中的关键字, 只支持英文
    static bool IsContainSigns(PSIHANDLE pFile, const std::vector<std::string>& signs, int headerSize);

private:
    //Linux下GNU Libc规定wchar_t为32位，所以不用wchar_t，改用char，然后转为16位的
    static std::string CharToChar16(const std::string& str);
};

}
