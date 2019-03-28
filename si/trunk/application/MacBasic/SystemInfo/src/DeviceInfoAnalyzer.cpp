#include"stdafx.h"
#include "SystemInfo/include/systeminfo/DeviceInfoAnalyzer.h"
#include "siutility/AutoUnitConvert.h"
#include "sidevice/PDeviceManager.h"
#include"DeviceEnumrator/PDeviceDef.h"
namespace macbasic
{


DeviceInfoAnalyzer::DeviceInfoAnalyzer()
{

}

DeviceInfoAnalyzer::~DeviceInfoAnalyzer()
{

}

bool DeviceInfoAnalyzer::analyze(CPRecord *rootRecord)
{
    pdevice:: DeviceDisks diskInfos;
    pdevice::CPDeviceManager::get()->getLocalDisks(diskInfos);
    CPRecordType* osType = CPRecordTypeFactory::get()->getType(L"table.sidisk");
    CPRecord* diskRecord = osType->newRecord();
    rootRecord->addChild(diskRecord);
    for(auto &diskInfo :diskInfos){
        CPRecord* osRecord = diskRecord->addChild();
        osRecord->at( L"Name") = diskInfo.displayName;
        osRecord->at(L"DeviceType") = diskInfo.deviceType;
        osRecord->at( L"Model") = diskInfo.model;
        osRecord->at( L"Vender") = diskInfo.vendor;
        //osRecord->at( L"Size") = diskInfo.size;
        osRecord->at(L"SerialNumber") =diskInfo.serial;
        osRecord->at(L"MachineID") =L"787899";
    }
    return true;
}

}
