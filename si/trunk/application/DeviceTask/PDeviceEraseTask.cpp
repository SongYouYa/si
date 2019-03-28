#include "stdafx.h"
#include "application/DeviceTask/PDeviceEraseTask.h"
#include "PSiDeviceTaskCommon.h"

#include "libdevtask/PDeviceTaskCommon.h"

#include "siio/PDeviceWriter.h"
#include "sidevice/PDiskHandler.h"

using namespace siio;
using namespace pdevice;
using namespace libdt;

namespace devtask
{
	void getDataSources(DeviceEraseType eraseType, std::vector<CPDataSourcePtr>& dataSources)
	{
		if (eraseType == ERASE_TYPE_SIMPLE){
			CPDataSourcePtr dataSource(new CPDataSource(CPDataSource::DT_FIXED, COMMON_BUF_LEN));
			dataSource->setFixedChar(0);
			dataSources.push_back(dataSource);
		}
		else if (eraseType == ERADE_TYPE_DOD) {
			//1
			CPDataSourcePtr dataSource1(new CPDataSource(CPDataSource::DT_FIXED, COMMON_BUF_LEN));
			uchar randChar1 = CPDataSource::getRandomChar();
			dataSource1->setFixedChar(randChar1);
			dataSources.push_back(dataSource1);
			//2
			CPDataSourcePtr dataSource2(new CPDataSource(CPDataSource::DT_FIXED, COMMON_BUF_LEN));
			dataSource2->setFixedChar(~randChar1);
			dataSources.push_back(dataSource2);
			//3
			CPDataSourcePtr dataSource3(new CPDataSource(CPDataSource::DT_RANDOM, COMMON_BUF_LEN));
			dataSources.push_back(dataSource3);
			////4
			CPDataSourcePtr dataSource4(new CPDataSource(CPDataSource::DT_FIXED, COMMON_BUF_LEN));
			uchar randChar4 = CPDataSource::getRandomChar();
			dataSource4->setFixedChar(randChar4);
			dataSources.push_back(dataSource4);
			//5、6、7和1、2、3一样
			CPDataSourcePtr dataSource5(new CPDataSource(CPDataSource::DT_FIXED, COMMON_BUF_LEN));
			uchar randChar5 = CPDataSource::getRandomChar();
			dataSource5->setFixedChar(randChar5);
			dataSources.push_back(dataSource5);

			CPDataSourcePtr dataSource6(new CPDataSource(CPDataSource::DT_FIXED, COMMON_BUF_LEN));
			dataSource6->setFixedChar(~randChar5);
			dataSources.push_back(dataSource6);

			CPDataSourcePtr dataSource7(new CPDataSource(CPDataSource::DT_RANDOM, COMMON_BUF_LEN));
			dataSources.push_back(dataSource7);
		}
		else if (eraseType == ERASE_TYPE_BMB) {
			CPDataSourcePtr dataSource1(new CPDataSource(CPDataSource::DT_FIXED, COMMON_BUF_LEN));
			dataSource1->setFixedChar(0);
			dataSources.push_back(dataSource1);

			CPDataSourcePtr dataSource2(new CPDataSource(CPDataSource::DT_FIXED, COMMON_BUF_LEN));
			dataSource2->setFixedChar(~0);
			dataSources.push_back(dataSource2);

			CPDataSourcePtr dataSource3(new CPDataSource(CPDataSource::DT_RANDOM, COMMON_BUF_LEN));
			dataSources.push_back(dataSource3);

			CPDataSourcePtr dataSource4(new CPDataSource(CPDataSource::DT_RANDOM, COMMON_BUF_LEN));
			dataSources.push_back(dataSource4);

			CPDataSourcePtr dataSource5(new CPDataSource(CPDataSource::DT_RANDOM, COMMON_BUF_LEN));
			dataSources.push_back(dataSource5);

			CPDataSourcePtr dataSource6(new CPDataSource(CPDataSource::DT_FIXED, COMMON_BUF_LEN));
			dataSource6->setFixedChar(0);
			dataSources.push_back(dataSource6);
		}
	}

	CPDeviceEraseTask::CPDeviceEraseTask()
	{

	}

	void CPDeviceEraseTask::doTask(CPTask* task)
	{
		TaskParamPtr param = task->param();
		CPTaskStatusCtrl* statusCtrl = task->statusCtrl();
		DeviceEraseParam eraseParam = boost::any_cast<DeviceEraseParam>(*param);

		DeviceDisk disk;
		CPDeviceManager::get()->getDiskByName(eraseParam.targetDevice.deviceName, disk);
		CPDeviceWriterPtr writer = std::make_shared<CPDeviceWriter>();
        if (!writer->open(disk.path)) {
            if (!CPSiDeviceTaskCommon::onOpenWriterFailed(disk.path, writer, statusCtrl)) {
                return;
            }
        }
		if (!writer->init()) {
			statusCtrl->fail(TASK_ERROR_INIT_DEVICE);
			PLOG(PLOG_ERROR, L"磁盘初始化错误");
			return;
		}
		writer->close();

		CPRecord* record = task->rootRecord();
		record->at(L"target_size") = disk.size;

		std::vector<CPDataSourcePtr> dataSources;
		getDataSources(eraseParam.eraseType, dataSources);

		size_t eraseCount = dataSources.size();
		CPTaskProgress* progress = statusCtrl->progress();
		progress->setSizePreCallback([eraseCount](uint64 curSize){
			return curSize / eraseCount;
		});
		uint64 totalEraseSize = disk.size * dataSources.size();
		progress->setTotalSize(disk.size);

		CPDeviceTaskCommon::EraseOffset eraseOffset;
		eraseOffset.startOffset = 0;
		eraseOffset.endOffset = disk.size;

        std::atomic<uint64> totalWrited(0);
		for (auto& dataSource : dataSources) {
			std::thread t(std::bind(&CPDeviceTaskCommon::eraseDevice,
				disk.path, eraseOffset, dataSource, statusCtrl, totalEraseSize, &totalWrited));
			t.join();
		}

		record->at(L"current_size") = progress->currentSize();

		//引发界面更新
		//CPDeviceManager::get()->update();
	}

}
