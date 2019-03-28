#include "stdafx.h"
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/bzip2.hpp> //bzip2_compressor
#include <boost/iostreams/device/file.hpp> //file_sink
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "boost/regex.hpp"
#include "PSysLogAnallyzer.h"

#include "siutility/PCodeConverter.h"

namespace macbasic {

CSysLogAnallyzer::CSysLogAnallyzer()
{
}

CSysLogAnallyzer::~CSysLogAnallyzer()
{
}

void CSysLogAnallyzer::GetComputerBootInfo(ComputerBootInfos& infos)
{
    infos = m_vecSysInfo;
}

bool CSysLogAnallyzer::ParseLogFile(const std::wstring &wstrFile)
{
    if (wstrFile.empty()) {
        return false;
    }
    std::string szPath = CPCodeChanger::UnicodeToUTF8(wstrFile.c_str());
    try {
        // mac 10.10
        if (std::string::npos != szPath.find(".gz")) {
            boost::iostreams::filtering_istream in;
            in.push(boost::iostreams::gzip_decompressor());//gzip InputFilter
            boost::iostreams::file_source source(szPath);
            in.push(source);

            if (!source.is_open()) {
                return false;
            }
            ReadSysLog((std::ifstream *)&in); //
            source.close();
        }
        //mac 10.8
        else if (std::string::npos != szPath.find(".bz")) {
            boost::iostreams::filtering_istream in;
            in.push(boost::iostreams::bzip2_compressor()); // bzip
            boost::iostreams::file_source source(szPath);
            in.push(source);

            if (!source.is_open()) {
                return false;
            }
            ReadSysLog((std::ifstream *)&in);
            source.close();
        }
        else {
            std::ifstream iff(szPath.c_str(), std::ios_base::in);
            if (!iff.is_open()) {
                return false;
            }
            ReadSysLog(&iff);
            iff.close();
        }
    }
    catch (std::runtime_error e) {
        std::cout << e.what();
        return false;
    }
    return !m_vecSysInfo.empty();
}

bool CSysLogAnallyzer::ReadSysLog(std::ifstream * in)
{
    std::string strLine;
    try {

        std::string strShutDownUser = "shutdown\\[(\\d+)\\]:[\\s]+halt[\\s]+by[\\s]+(\\w+):";
        std::string strShutDownTime = "shutdown\\[(\\d+)\\]:[\\s]+SHUTDOWN_TIME:[\\s]+(\\d+)[\\s]+(\\d+)";
        std::string strBoot         = "bootlog\\[(\\d+)\\]:[\\s]+BOOT_TIME[\\s]+(\\d+)[\\s]+(\\d+)";
        std::string strReset        = "shutdown\\[(\\d+)\\]:[\\s]+reboot[\\s]+by[\\s]+(\\w+):";

        boost::regex regSU(strShutDownUser);
        boost::regex regST(strShutDownTime);
        boost::regex regBt(strBoot);
        boost::regex regRs(strReset);

        ComputerBootInfo shutdownInfo;
        int proNo = -1;
        while(std::getline(*in, strLine))
        {
//            std::cout << strLine << std::endl;
            if (strLine.empty()) {
                continue;
            }

            boost::cmatch what;
            ComputerBootInfo sysinfo;
            if (boost::regex_search(strLine.c_str(), what, regSU)) {
                shutdownInfo.wstrEvent = SHUTDOWN;
                proNo = std::atoi(what[1].str().c_str());
                shutdownInfo.wstrName = CPCodeChanger::UTF8ToUnicode(what[2].str().c_str());
            }
            else if(boost::regex_search(strLine.c_str(), what, regST))
            {
                if (proNo == std::atoi(what[1].str().c_str())) {
                    StrToTime(what[2].str(), shutdownInfo.logTime);
                    m_vecSysInfo.push_back(shutdownInfo);
                }
            }
            else if (boost::regex_search(strLine.c_str(), what, regBt)) {
                sysinfo.wstrEvent = BOOT;
                StrToTime(what[2].str(), sysinfo.logTime);
                m_vecSysInfo.push_back(sysinfo);
            }
            else if (boost::regex_search(strLine.c_str(), what, regRs)) {
                shutdownInfo.wstrEvent = RESET;
                proNo = std::atoi(what[1].str().c_str());
                shutdownInfo.wstrName = CPCodeChanger::UTF8ToUnicode(what[2].str().c_str());
            }
        }
    }
    catch (...) {
        return false;
    }
    return true;
}

void CSysLogAnallyzer::StrToTime(std::string str,CPTime & time)
{
    time_t ttTime = std::atof(str.c_str());
    time = CPTime::UTC2LocalTime(ttTime, siutil::MacCommonHelper::GetTimezoneRegion());
}

}
