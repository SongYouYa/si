#pragma once
namespace app
{
    using namespace std;
    enum OSType
    {
        OS_UNKNOWN = 0,
        OS_WINDOWS = 1,
        OS_LINUX = 2
    };
    struct OperationSystem
    {
        std::wstring name_;
        OSType os_type_;
    };
    struct WindowsOS : public OperationSystem
    {
        std::wstring user_directory_path_;
        std::wstring windows_directory_path_;
    public:
        std::wstring GetUserRegisterPath()
        {
            return user_directory_path_;
        }
        std::wstring GetSystemRegisterPath()
        {
            std::wstring system_register_path = windows_directory_path_;
            
            if (name_ == L"WindowsXP")
            {
                return system_register_path.append(L"/system32/config");//C:\Windows\system32\config;
            }
            else
            {
                return system_register_path.append(L"/System32/config");//C:\Windows\System32\config;
            }
        }
    };
    class  OSLocater
    {
    public:
        OSLocater();
        ~OSLocater();
    public:
        void GetAllOperationSystemInfo(std::vector<boost::shared_ptr<WindowsOS> >& all_windows_os);
        bool GetPartitionOSInformation(const wstring path, vector<boost::shared_ptr<WindowsOS> >& oses);
        bool GetNoSystemPartitions(const std::vector<std::wstring>& all_partition, std::vector<std::wstring>& no_system_partition);
        bool GetUserDirectoryPath(const std::vector<std::wstring>& all_partition, std::vector<std::wstring>& user_directory_path);
        // 获取子文件夹(张文攀，20150127)
        bool GetSubDirectory(const std::wstring& rootdir, std::vector<std::wstring>& user_directory_path);
    private:
        class OSLocateImp;
        boost::shared_ptr<OSLocateImp> os_locater_impl_;
    };
}

