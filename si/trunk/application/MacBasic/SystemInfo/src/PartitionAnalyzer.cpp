#include "stdafx.h"
#include "SystemInfo/include/systeminfo/PartitionAnalyzer.h"
#include "siutility/AutoUnitConvert.h"
#include "filesystemshell/FileSystemShell.h"
#include "sidevice/PDeviceManager.h"

#define SI_MAC
#ifdef SI_MAC
#include "siutility/MacCommonHelper.h"
#endif

namespace macbasic{

PartitionAnalyzer::PartitionAnalyzer()
{

}

PartitionAnalyzer::~PartitionAnalyzer()
{

}

bool PartitionAnalyzer::analyze(CPRecord *rootRecord)
{

    pdevice::DeviceDisks localDisks;
    pdevice::CPDeviceManager::get()->getLocalDisks(localDisks);
    std::vector<PSIHANDLE> allVolumes;
    filesystem::CFileSystem::GetAllVolumes(allVolumes);
    CPRecordType* diskType = CPRecordTypeFactory::get()->getType(L"table.sipartition");
    CPRecord* diskRecord = diskType->newRecord();
    rootRecord->addChild(diskRecord);

    for(auto &localDisk :localDisks){
        for(auto & partition :localDisk.partitions){
            CPRecord* osRecord = diskRecord->addChild();
            osRecord->at(L"SIPartitionName") = partition.name;
            osRecord->at(L"SIFileSystem") = partition.filesystem;
            std::wstring unit_size = siutility::AutoUnitConvert::BytesToUintString(partition.size);
            osRecord->at(L"SISize") = unit_size;

//            for(std::vector<PSIHANDLE>::iterator it = allVolumes.begin(); it != allVolumes.end(); ++it)
//            {
//                PSIHANDLE pVol = *it;
//                std::wstring vPath = filesystem::CFileSystem::GetItemPath(pVol);
////                if(vPath == partition.mountPoints.at(0))
////                {
////                    if(siutility::MacCommonHelper::IsMacSystemVolume(pVol))
////                    {
////                        osRecord->at(L"SIOperate") = std::wstring(L"OS X");
////                    }
////                    break;
////                }
//            }
        }
    }
    //#else
    //        osRecord->at(L"Name") = partition.display_name_;
    //        osRecord->at(L"MountedPoint") = partition.mount_point_;
    //        osRecord->at(L"FileSystem") = partition.filesystem_;
    //        osRecord->at(L"Size") = utility::AutoUnitConvert::BytesToUintString(partition.size_);
    //        osRecord->at(L"Available") = utility::AutoUnitConvert::BytesToUintString(partition.available_);
    //#endif
    return true;
}


}
