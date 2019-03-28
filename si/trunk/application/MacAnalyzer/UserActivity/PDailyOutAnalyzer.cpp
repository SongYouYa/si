#include "stdafx.h"
#include "PDailyOutAnalyzer.h"
#include <boost/regex.hpp>
#include <boost/regex/pattern_except.hpp>
#include <QDebug>
#include "siutility/PCodeConverter.h"
namespace macbasic {

CPDailyOutAnalyzer::CPDailyOutAnalyzer()
{
}

CPDailyOutAnalyzer::~CPDailyOutAnalyzer()
{
    m_vecDiskInfo.clear();
    m_vecNetInfo.clear();
    m_vecDailyFile.clear();
}

std::vector<DiskStatusInfo> &CPDailyOutAnalyzer::GetDiskVector()
{
    return m_vecDiskInfo;
}

std::vector<NetworkInfo> &CPDailyOutAnalyzer::GetNetwVector()
{
    return m_vecNetInfo;
}

bool CPDailyOutAnalyzer::AddDailyOutFile(const std::wstring &wstrFile)
{
    if (wstrFile.empty()) {
        return false;
    }
    m_vecDailyFile.push_back(wstrFile);
    if (wstrFile != m_vecDailyFile.back()) {
        return false;
    }
    return DailyOutAnalyzer(wstrFile);
}

bool CPDailyOutAnalyzer::DailyOutAnalyzer(const std::wstring &wstrFile)
{
    if (wstrFile.empty()) {
        return false;
    }
    std::string szPath = CPCodeChanger::UnicodeToUTF8(wstrFile.c_str());
    try {
        std::ifstream iff(szPath.c_str(), std::ios_base::in);
        if (!iff.is_open()) {
            return false;
        }

        std::string strLine;
        while(std::getline(iff, strLine)) {
            if (strLine.empty()) {
                continue;
            }
            struct tm time;
            if (!GetDailyOutTime(strLine, time)) {
                continue;
            }
            time_t ttTime = mktime(&time);
          CPTime cTime = CPTime::UTC2LocalTime(ttTime, siutil::MacCommonHelper::GetTimezoneRegion());

            while(std::getline(iff, strLine)) {
                // disk
                if (std::string::npos != strLine.find("Disk status:")) {
                    std::getline(iff, strLine);
                    while(std::getline(iff, strLine)) {
                        if (strLine.empty()) {
                            break;
                        }
                        GetDiskStatusInfo(strLine, cTime);
                    }
                }
                // network
                else if (std::string::npos != strLine.find("Network interface status:")) {
                    std::getline(iff, strLine);
                    while(std::getline(iff, strLine)) {
                        if (strLine.empty()) {
                            break;
                        }
                        GetNetwStatusInfo(strLine, cTime);
                    }
                }
                // end
                else if(std::string::npos != strLine.find("-- End of daily output --")) {
                    break;
                }
            }
        }
    }
    catch (...) {
        return false;
    }
    return true;
}

bool CPDailyOutAnalyzer::GetDailyOutTime(const std::string & strLine, tm & time)
{
    try {
        std::string str = "([\\w]+)";
        std::string num = "([\\d]+)";
        boost::regex reg(str + "[\\s]+" + str + "[\\s]+" + num + "[\\s]+" +
                         num + "[:]" + num + "[:]" + num + "[\\s]+" + str + "[\\s]+" + num);
        boost::cmatch what;
        if (boost::regex_match(strLine.c_str(), what, reg)) {
            time.tm_mon     = monthStr2Int(what[2].str().c_str()) - 1;
            time.tm_mday    = std::atoi(what[3].str().c_str());
            time.tm_hour    = std::atoi(what[4].str().c_str());
            time.tm_min     = std::atoi(what[5].str().c_str());
            time.tm_sec     = std::atoi(what[6].str().c_str());
            time.tm_year    = std::atoi(what[8].str().c_str()) - 1900;
        }
    }
    catch (...) {
        return false;
    }
    return true;
}

bool CPDailyOutAnalyzer::GetDiskStatusInfo(const std::string &strLine, const CPTime & cTime)
{
    try {
        std::string reg_filesystem  = "([\\/|\\w|\\d]+)";
        std::string reg_kbs         = "([\\.|\\w|\\d]+)";
        std::string reg_percent     = "([\\d]+%)";
        std::string reg_mounted     = "(\\/.*)";

        boost::regex reg(reg_filesystem + "[\\s]+" + reg_kbs + "[\\s]+" + reg_kbs + "[\\s]+" + reg_kbs
                         + "[\\s]+" + reg_percent + "[\\s]+" + reg_kbs + "[\\s]+" + reg_kbs + "[\\s]+"
                         + reg_percent + "[\\s]+" + reg_mounted);
        boost::cmatch what;
        if (boost::regex_match(strLine.c_str(), what, reg)) {
            DiskStatusInfo disk;
            disk.diskTime       = cTime;
            disk.fileSystem     = CPCodeChanger::UTF8ToUnicode(what[1].str().c_str());
            disk.size           = CPCodeChanger::UTF8ToUnicode(what[2].str().c_str());
            disk.used           = CPCodeChanger::UTF8ToUnicode(what[3].str().c_str());
            disk.avail          = CPCodeChanger::UTF8ToUnicode(what[4].str().c_str());
            disk.capacity       = CPCodeChanger::UTF8ToUnicode(what[5].str().c_str());
            disk.iused          = CPCodeChanger::UTF8ToUnicode(what[6].str().c_str());
            disk.ifree          = CPCodeChanger::UTF8ToUnicode(what[7].str().c_str());
            disk.usedPercent    = CPCodeChanger::UTF8ToUnicode(what[8].str().c_str());
            disk.mounted        = CPCodeChanger::UTF8ToUnicode(what[9].str().c_str());

            m_vecDiskInfo.push_back(disk);
        }
    }
    catch (...) {
        return false;
    }
    return true;
}

bool CPDailyOutAnalyzer::GetNetwStatusInfo(const std::string &strLine, const CPTime & cTime)
{
    try {
        std::string reg_name    = "([\\w|\\d|\\*]+)";
        std::string reg_mtu     = "([\\d]+)";
        std::string reg_network = "([<|>|\\w|\\d|\\#|:|\\.|/|%]+)";
        std::string reg_address = "(\\s|[\\w|\\d|:|\\.]+)";
        std::string reg_pkts    = "(-|[\\d]+)";

        boost::regex reg(reg_name + "[\\s]+" + reg_mtu + "[\\s]+" + reg_network + "[\\s]+" + reg_address
                         + "[\\s]+" + reg_pkts+ "[\\s]+" + reg_pkts+ "[\\s]+" + reg_pkts+ "[\\s]+" +
                         reg_pkts + "[\\s]+" + reg_pkts);
        boost::cmatch what;
        if (boost::regex_match(strLine.c_str(), what, reg)) {
            NetworkInfo netw;
            netw.networkTime    = cTime;
            netw.name           = CPCodeChanger::UTF8ToUnicode(what[1].str().c_str());
            netw.mtu            = CPCodeChanger::UTF8ToUnicode(what[2].str().c_str());
            netw.network        = CPCodeChanger::UTF8ToUnicode(what[3].str().c_str());
            netw.address        = CPCodeChanger::UTF8ToUnicode(what[4].str().c_str());
            netw.ipkts          = CPCodeChanger::UTF8ToUnicode(what[5].str().c_str());
            netw.ierrs          = CPCodeChanger::UTF8ToUnicode(what[6].str().c_str());
            netw.opkts          = CPCodeChanger::UTF8ToUnicode(what[7].str().c_str());
            netw.oerrs          = CPCodeChanger::UTF8ToUnicode(what[8].str().c_str());
            netw.coll           = CPCodeChanger::UTF8ToUnicode(what[9].str().c_str());

    //        std::cout << what[3].str().c_str() << std::endl;
            m_vecNetInfo.push_back(netw);
        }
    }
    catch (...) {
        return false;
    }
    return true;
}

int CPDailyOutAnalyzer::monthStr2Int(std::string str)
{
    if ("Jan" == str) {
        return 1;
    }
    else if ("Feb" == str) {
        return 2;
    }
    else if ("Mar" == str) {
        return 3;
    }
    else if ("Apr" == str) {
        return 4;
    }
    else if ("May" == str) {
        return 5;
    }
    else if ("Jun" == str) {
        return 6;
    }
    else if ("Jul" == str) {
        return 7;
    }
    else if ("Aug" == str) {
        return 8;
    }
    else if ("Sep" == str) {
        return 9;
    }
    else if ("Oct" == str) {
        return 10;
    }
    else if ("Nov" == str) {
        return 11;
    }
    else if ("Dec" == str) {
        return 12;
    }
    else {
        return -1;
    }
}

}
