#ifndef SYSTEMINFO_DEVICEINFOANALYZER_H
#define SYSTEMINFO_DEVICEINFOANALYZER_H

namespace macbasic
{
	class DeviceInfoAnalyzer
	{
	public:
		DeviceInfoAnalyzer();
		~DeviceInfoAnalyzer();

       static bool analyze(CPRecord *rootRecord);
	};
}

#endif
