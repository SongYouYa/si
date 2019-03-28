#pragma once

namespace app
{
    class TimeConverter
    {
    public:
        //  static FILETIME DataTimeToFileTime(const std::wstring& bstr);
        static time_t  FileTimeToTimet(const FILETIME& file_time);
        static time_t LargeIntegerToTimet(const LARGE_INTEGER& large_integer);
        static int TimerStringToInt(std::string timer); //hh::mm::ss;
        static time_t SystemTimeToTimet(const SYSTEMTIME& system_time);
    };
}
