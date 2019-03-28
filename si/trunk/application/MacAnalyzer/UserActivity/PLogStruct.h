#ifndef PLOGSTRUCT_H
#define PLOGSTRUCT_H
#include "base/PTime.h"
#include "base/PRecord.h"
#include <vector>
//#define MAC_BASE_TIME 978307200

#define BOOT        L"开机"
#define RESET       L"重启"
#define SHUTDOWN    L"关机"

namespace macbasic {

// 开关机信息
struct ComputerBootInfo{
    std::wstring    wstrEvent;
    CPTime    logTime;
    std::wstring    wstrName;

    bool operator !=(const ComputerBootInfo& rh)
    {
        if(wstrEvent != rh.wstrEvent ||
                logTime != rh.logTime ||
                wstrName != rh.wstrName)
        {
            return true;
        }
        return false;
    }

    void AddToTable(CPRecord *rootRecoard)
    {
        CPRecord *table = rootRecoard->addChild();
        table->at( L"Event")    = wstrEvent;
        table->at(L"Time")  = logTime;
        table->at(L"UserName")     = wstrName;
    }
} ;
typedef std::vector<ComputerBootInfo> ComputerBootInfos;

//// system.log中一条记录包含的信息

// 磁盘状态
typedef struct {
    CPTime    diskTime;
    std::wstring    fileSystem;
    std::wstring    size;
    std::wstring    used;
    std::wstring    avail;
    std::wstring    capacity;
    std::wstring    iused;
    std::wstring    ifree;
    std::wstring    usedPercent;
    std::wstring    mounted;

    void AddToTable(CPRecord *rootRecord)
    {
       CPRecord *table = rootRecord->addChild();
        table->at(L"Time")         = diskTime;
        table->at(L"FileSystem")   = fileSystem;
        table->at(L"Size")         = size;
        table->at(L"Used")         = used;
        table->at(L"Avail")        = avail;
        table->at(L"Capacity")     = capacity;
        table->at(L"Iused")        = iused;
        table->at(L"Ifree")        = ifree;
        table->at(L"UsedPercent")  = usedPercent;
        table->at(L"Mounted")      = mounted;
    }

} DiskStatusInfo;
typedef std::vector<DiskStatusInfo> DiskStatusInfos;

// 网络接口状态
typedef struct {
    CPTime    networkTime;
    std::wstring    name;
    std::wstring    mtu;
    std::wstring    network;
    std::wstring    address;
    std::wstring    ipkts;
    std::wstring    ierrs;
    std::wstring    opkts;
    std::wstring    oerrs;
    std::wstring    coll;

    void AddToTable(CPRecord *rootRecord)
    {
        CPRecord *table = rootRecord->addChild();
        table->at(L"Time")     = networkTime;
        table->at(L"Name")     = name;
        table->at(L"Mtu")      = mtu;
        table->at(L"Network")  = network;
        table->at(L"Address")  = address;
        table->at(L"Ipkts")    = ipkts;
        table->at(L"Ierrs")    = ierrs;
        table->at(L"Opkts")    = opkts;
        table->at(L"Oerrs")    = oerrs;
        table->at(L"Coll")     = coll;
    }
} NetworkInfo;
typedef std::vector<NetworkInfo> NetworkInfos;

}

#endif // PLOGSTRUCT_H
