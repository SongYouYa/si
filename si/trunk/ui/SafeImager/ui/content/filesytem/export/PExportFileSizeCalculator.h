#pragma once
#include "application/DeviceTask/PDeviceTaskParam.h"

//生成父节点和子节点的map
class CPExportFileSizeCalculator
{
public:
	CPExportFileSizeCalculator();
	void  update(const std::vector<PSIHANDLE>& rootHandles);
	uint64 getSingleSize();
	uint64 getTotalSize();

private:
	//第三个参数判断每一个文件的大小是否符合要求
	void insert(PSIHANDLE parent);
	uint64 _totalSize;
	uint64 _largestSingleSize;
};
