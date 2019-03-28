#include "stdafx.h"
#include "TimeConverter.h"

namespace app
{
    time_t TimeConverter::FileTimeToTimet(const FILETIME& file_time)
    {
        //long long temp;
        ULARGE_INTEGER ui;
        ui.LowPart = file_time.dwLowDateTime;
        ui.HighPart = file_time.dwHighDateTime;
        time_t  timet = ((long long )(ui.QuadPart - 116444736000000000) / 10000000);
        return timet;
    }
    time_t TimeConverter::LargeIntegerToTimet(const LARGE_INTEGER& large_integer)
    {
        time_t t = ((long long)(large_integer.QuadPart - 116444736000000000) / 10000000);
        return t;
    }

    int TimeConverter::TimerStringToInt(std::string timer)
    {
        int hour = boost::lexical_cast<int>(timer.substr(0, 2));
        int minute = boost::lexical_cast<int>(timer.substr(3, 2));
        int second =boost::lexical_cast<int>(timer.substr(6, 2));
        int total_seconds = hour* 3600 + minute*60 + second;
        return total_seconds;
    }

    time_t TimeConverter::SystemTimeToTimet(const SYSTEMTIME& system_time)
    {
        FILETIME ft;
        SystemTimeToFileTime(&system_time, &ft);
        return FileTimeToTimet(ft);
    }
}
