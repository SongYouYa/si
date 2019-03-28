#pragma once
#include "application/DeviceTask/PDeviceTaskParam.h"

//���ɸ��ڵ���ӽڵ��map
class CPExportFileSizeCalculator
{
public:
	CPExportFileSizeCalculator();
	void  update(const std::vector<PSIHANDLE>& rootHandles);
	uint64 getSingleSize();
	uint64 getTotalSize();

private:
	//�����������ж�ÿһ���ļ��Ĵ�С�Ƿ����Ҫ��
	void insert(PSIHANDLE parent);
	uint64 _totalSize;
	uint64 _largestSingleSize;
};
