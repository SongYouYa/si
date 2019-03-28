#include "stdafx.h"
#include "sifile/PMemoryReader.h"

#include <fcntl.h>

#include "sicommon/PProcess.h"

namespace siio
{
std::wstring CPMemoryReader::s_displayName = L"";
std::wstring CPMemoryReader::s_macDriverPath = L"";
static uint64 s_memorySize = 0;

class CPMemoryReader::Impl
{
public:
    Impl()
        :_handle(-1)
    {}

    int _handle;
};

CPMemoryReader::CPMemoryReader()
    : _impl(new Impl)
{

}

CPMemoryReader::~CPMemoryReader()
{
    close();
    if (_impl) {
        delete _impl;
    }
}

bool CPMemoryReader::open(const std::wstring& path, int openMode)
{
    //打开设备
    if(!initPmemDriver()){
        return false;
    }

    if(( _impl->_handle = ::open("/dev/pmem",openMode)) < 0) {
        printf("errno=%d\n",errno);
        char * mesg = strerror(errno);
        printf("Mesg:%s\n", mesg);
        return false;
    }
    else {
        //unknow
        return true;
    }
}

bool CPMemoryReader::initPmemDriver()
{
    //返回利用脚本获取的系统内存大小
    int exitCode = 0;
    //判断系统是否禁掉系统保护添加判断
    std::string output;
    std::wstring  csrutilcmd = L"csrutil status";
    bool ret = CPProcess::createProcess(csrutilcmd, L"", &output, &exitCode);
    if (!ret || exitCode != 0) {
        return false;
    }
    std::string result = "System Integrity Protection status: disabled.\n";
    if(result != output){
        //系统没有关闭硬件保护选项
        return false;
    }

    //修改权限
    std::wstring chmodCmd = L"sudo chown -R root:wheel " + s_macDriverPath;
    ret = CPProcess::createProcess(chmodCmd, L"", NULL, &exitCode);
    if (!ret || exitCode != 0) {
        return false;
    }
    std::wstring modifiPermitCmd  = L"sudo chmod -R 755 " + s_macDriverPath;
    ret = CPProcess::createProcess(modifiPermitCmd, L"", NULL, &exitCode);
    if (!ret || exitCode != 0) {
        return false;
    }
    //加载驱动
    std::wstring loadCmd = L"sudo kextload " + s_macDriverPath;
    ret = CPProcess::createProcess(loadCmd, L"", NULL, &exitCode);
    if (!ret || exitCode != 0) {
        return false;
    }
    return true;
}

uint64 CPMemoryReader::size()
{
    //预防第二次获取大小的时候影响效率
    if(s_memorySize != 0){
        return s_memorySize;
    }
    //返回最后最后的开始位置和偏移量之和
    std::wstring cmdBeginPos = L"sudo cat /dev/pmem_info |tail -n 3|cut -d : -f2|cut -d ' ' -f2|sed -n 1p";
    std::wstring cmdOffPos = L"sudo cat /dev/pmem_info |tail -n 3|cut -d : -f2|cut -d ' ' -f2|sed -n 2p";
    std::string pipe_buf;
    int exitCode = 0;
    int ret = CPProcess::createProcess(cmdBeginPos, L"", &pipe_buf, &exitCode);
    if (!ret || exitCode != 0) {
        return false;
    }
    uint64 beginsizefromint = std::atof(pipe_buf.c_str());
    std::string pipe_buf_value;
    ret = CPProcess::createProcess(cmdOffPos, L"", &pipe_buf_value, &exitCode);
    if (!ret || exitCode != 0) {
        return false;
    }
    uint64 offsizefromint = std::atof(pipe_buf_value.c_str());
    //此操作占用很多时间 因此保存一个变量
    s_memorySize = beginsizefromint + offsizefromint;
    return s_memorySize;
}

CPFile::ErrorCode CPMemoryReader::read(void* buf, uint len, uint* readed)
{
    int tempReaded = ::read(_impl->_handle, buf, len);
    if (tempReaded < 0) {
        //may be dev lost, or bad track
        return CPFile::PERROR_UNKOWN;
    }
    if (readed) {
        *readed = tempReaded;
    }
    return CPFile::PERROR_SUCCESS;
}

bool CPMemoryReader::seek(uint64 len, PSeekFrom from, uint64* seeked)
{
    if (len == 0) {
        //内存设备不支持seek，但如果seek长度为0，为保持程序接口一致，返回true
        return true;
    }
    else {
        return false;
    }
}

void CPMemoryReader::close()
{
    if (_impl->_handle >= 0) {
        ::close(_impl->_handle);
        _impl->_handle = -1;
        //卸载驱动
        int exitCode;
        CPProcess::createProcess(L"sudo kextunload -b com.google.MacPmem ", L"", NULL, &exitCode);
    }
}

void CPMemoryReader::setDisplayName(const std::wstring& displayName)
{
    s_displayName = displayName;
}

std::wstring CPMemoryReader::getDisplayName()
{
    return s_displayName;
}

void CPMemoryReader::setDriverPath(const std::wstring &path)
{
    s_macDriverPath = path;
}

}
