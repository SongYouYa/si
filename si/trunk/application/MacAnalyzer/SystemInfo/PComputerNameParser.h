#ifndef GETCOMPUTERNAME_H
#define GETCOMPUTERNAME_H
#include <vector>
#include <iostream>

namespace macbasic {

class  CPComputerNameGeting
{

public:
        CPComputerNameGeting();
        ~CPComputerNameGeting();
        static bool analyze(CPRecord* rootRecord);
        static bool AnalyzerPlist(const std::wstring& plistpath, std::wstring &computerName);
        std::vector<std::wstring>m_computerNamepicked;
};
}
#endif
