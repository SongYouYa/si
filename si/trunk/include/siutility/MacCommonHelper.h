#ifndef MACOMMONHELPER_
#define MACOMMONHELPER_

namespace siutil
{
class  MacCommonHelper
{
public:
    MacCommonHelper();
    ~MacCommonHelper();
    //===========================================================
    // brief :		获取mac下的所有用户名
    // Returns:
    // Parameter:	setUsers	保存所有用户名
    // Author:    	jiaowei
    // DATE:      	2015.8.3 18:10:18
    //===========================================================
    static void GetMacAllUserName(std::set<std::wstring> & setUsers);

    //通过“用户目录”下的相对路径，获取所有匹配的handle
    static void GetHandlesByUserRawPath(const std::wstring& userRawRootPath,
                                        const std::wstring& fileName,
                                        std::vector<PSIHANDLE>& handles);

    //获取mac系统所设的时区地名，如"Asia/Shanghai", 失败则返回空
    static std::string GetTimezoneRegion();
    //在特定卷下，查找时区文件
    static std::string GetTimezoneRegion(PSIHANDLE pVolume);
    // 判断是否mac系统卷
    static bool IsMacSystemVolume(PSIHANDLE pVolume);
private:
    //返回时区差，单位为分钟，如东8区，则为-480
    //        static long GetTimezoneBias();                   // 时区解析不太对，暂不使用, 代码先保留
    static std::string GetTimeZoneFilePath(PSIHANDLE pVolume);
    // 解析lnk文件的真实路径
    static std::string GetRealPath(std::wstring lnkPath);
    // 获取某个handle所在卷的挂载路径
    static std::wstring GetMountPath(PSIHANDLE handle);

private:
    static std::set<std::wstring> setUserNames_;

    static std::mutex tz_mu_;
    static std::string tiemzoneRegion_;
    //        static bool timezoneParsed_; //时区是否已分析过
    //        static long bias_;
};
}
#endif
