#pragma once

namespace pfile {

class CommonCheck
{
public:
    CommonCheck();

    // ˳�����ļ����Ƿ����signs�еĹؼ���, ֻ֧��Ӣ��
    static bool IsContainSigns(PSIHANDLE pFile, const std::vector<std::string>& signs, int headerSize);

private:
    //Linux��GNU Libc�涨wchar_tΪ32λ�����Բ���wchar_t������char��Ȼ��תΪ16λ��
    static std::string CharToChar16(const std::string& str);
};

}
