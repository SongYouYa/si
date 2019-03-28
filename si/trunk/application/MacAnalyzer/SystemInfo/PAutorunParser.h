#pragma once
#include "PAutorunStruct.h"
class CPRecord;

namespace macbasic {
class CPAutorunParser
{
public:
    ~CPAutorunParser();

    bool GetAutorunInfo(CPRecord *table);

private:
    //内核扩展模块
    void GetKext(AutoRunInfos& allKextInfo);
    bool ParseKext(PSIHANDLE plistHandle, AutoRunInfo& kextInfo);
    //launchctl调用
    void GetLaunchctl(AutoRunInfos& launchctlInfos);
    bool ParseLaunchctl(PSIHANDLE handle, AutoRunInfo& lauchInfo);
    //mach_init.d
    void GetMachInit(AutoRunInfos& machinitInfos);
    bool ParseMachInit(PSIHANDLE plistHandle, AutoRunInfo& machinitInfo);
    //LaunchDaemon
    void GetLauchDaemons(AutoRunInfos& lauchDaemonsInfos);
    //systemstarter
    void GetSystemStarter(AutoRunInfos& systemStarterInfos);
    void GetSystemStarterProg(AutoRunInfos& systemStarterInfos);
    void ParseSystemStarterProg(std::map<std::wstring, std::wstring> progparams, AutoRunInfos& systemStarterInfos);
    void GetSystemStarterScript(AutoRunInfos& systemStarterInfos);
    void GetSystemStarterItems(AutoRunInfos& systemStarterInfos);
    bool ParseSystemStarterItems(PSIHANDLE handle, AutoRunInfos& systemStarterInfos);
    //LaunchAgent
    void GetLaunchAgent(AutoRunInfos& agentInfos);
    bool ParseLaunchAgentRoot(PSIHANDLE rootHandle, AutoRunInfos& agentInfos);
    //LoginItem
    void GetLoginitems(AutoRunInfos& loginInfos);
    bool ParseLoginitem(PSIHANDLE plistHandle, AutoRunInfos& loginInfos);
};

}

