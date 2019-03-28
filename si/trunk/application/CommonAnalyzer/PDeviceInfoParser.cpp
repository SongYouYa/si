#include "stdafx.h"
#include "application/CommonAnalyzer/PDeviceInfoParser.h"

#include "DeviceEnumrator/PDeviceEnumrator.h"
#include "DeviceEnumrator/PDeviceUtils.h"
#include "sidevice/PDeviceManager.h"

using namespace pdevice;

namespace app
{
	bool CPDeviceInfoParser::getLocalDiskInfos(CPRecord* parentRecord)
	{
		DeviceDisks localDisks;
		CPDeviceEnumrator devEnumer;
		devEnumer.getALlLocalDisks(localDisks);

		for (auto& disk : localDisks) {
			CPRecord* record = parentRecord->addChild();
			record->at(L"name") = disk.displayName;
			record->at(L"model") = disk.vendor + L" " + disk.model;
			record->at(L"size") = disk.size;
			record->at(L"SerialNumber") = disk.serial;
			record->at(L"type") = CPDeviceUtils::getBusTypeStr(disk.busType);
		}
		return true;
	}

	void partitionsToRecord(const DevicePartitions& partitions, CPRecord* parentRecord)
	{
		for (auto& partition : partitions) {
			CPRecord* record = parentRecord->addChild();
			record->at(L"name") = partition.displayName;
			if (!partition.mountPoints.empty()) {
				record->at(L"MountPoint") = partition.mountPoints.at(0);
			}
			record->at(L"filesystem") = partition.filesystem;
			record->at(L"size") = partition.size;
			record->at(L"available") = partition.available;
		}
	}

	bool CPDeviceInfoParser::getLocalPartitionInfos(CPRecord* parentRecord)
	{
		DeviceDisks localDisks;
		CPDeviceManager::get()->getLocalDisks(localDisks);
		for (auto& disk : localDisks) {
			partitionsToRecord(disk.partitions, parentRecord);
		}

		DevicePartitions logicalPartitons;
        CPDeviceManager::get()->getLocalLogicalPartitions(logicalPartitons);
		partitionsToRecord(logicalPartitons, parentRecord);

		return true;
	}
}
