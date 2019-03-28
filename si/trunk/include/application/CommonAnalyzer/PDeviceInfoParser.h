#pragma once


namespace app
{
	class CPDeviceInfoParser
	{
	public:
		static bool getLocalDiskInfos(CPRecord* parentRecord);
		static bool getLocalPartitionInfos(CPRecord* parentRecord);
	};
}