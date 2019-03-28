#include "stdafx.h"
#include "application/DeviceTask/PDeviceCloneTask.h"
#include "PSiDeviceTaskCommon.h"

#include "application/DeviceTask/cache/PCloneTaskCache.h"
#include "./cache/PCloneTaskCacheImpl.h"

#include "libdevtask/PDeviceTaskCommon.h"
#include "libdevtask/PDeviceTaskUtils.h"

#include "siio/PDeviceWriter.h"
#include "sicommon/PUtils.h"

#ifdef OS_PWINDOWS
#include "sidevice/PDiskPartTool.h"
#endif
#include "sidevice/PDiskHandler.h"


using namespace pdevice;
using namespace siio;
using namespace libdt;

namespace devtask
{

	class CPDeviceCloneTask::Impl
	{
	public:
		Impl();
		~Impl();

	};

	CPDeviceCloneTask::Impl::Impl()
	{

	}

	CPDeviceCloneTask::Impl::~Impl()
	{

	}

	CPDeviceCloneTask::CPDeviceCloneTask()
		:_impl(nullptr)
	{

	}

	CPDeviceCloneTask::~CPDeviceCloneTask()
	{
		if (_impl) {
			delete _impl;
			_impl = nullptr;
		}
	}

	bool init(CPTask* task, CPCloneTaskCache* cloneCache, DeviceCloneParam& param, 
		CPHashComputerPtr& hasher,
		TDeviceParam& writeDeviceParam, CPDeviceWriterPtr& writer)
	{
		CPTaskStatusCtrl* statusCtrl = task->statusCtrl();
		
		CPCloneTaskCacheImpl* cacheImpl = cloneCache->getImpl();
		
		bool isCached = cloneCache->isInited(); //是否断点续传的任务
		if (isCached) {
			//如果任务已经初始化过，则是断点续传的任务
			if (!cacheImpl->iopen()) {
				statusCtrl->fail();
				return false;
			}
			param = cacheImpl->getParam();
			hasher = cacheImpl->getHasher();

			//恢复进度
			ProgressInfo progressInfo;
			cacheImpl->getProgressInfo(progressInfo);
			statusCtrl->progress()->setCachedTime(progressInfo.used_time);
			
		}
		else {
			//新任务
			TaskParamPtr taskParamPtr = task->param();
			param = boost::any_cast<DeviceCloneParam>(*taskParamPtr);
			hasher = std::make_shared<CPHashComputer>(param.hashTypes);
		}
		
		CPDeviceTaskUtils::getDeviceParam(param.targetDevice, writeDeviceParam);
		writer = std::make_shared<CPDeviceWriter>();
		if (!writer->open(writeDeviceParam.diskPath)) {
            if (!CPSiDeviceTaskCommon::onOpenWriterFailed(writeDeviceParam.diskPath,
                                                          writer, statusCtrl)) {
                return false;
            }
		}

		cacheImpl->setWriter(writer);

		if (!isCached) {
			//新的任务，需要先初始化磁盘，才能初始化缓存
			//因为初始化缓存时，需要向磁盘里写标志数据
			if (!writer->init()) {
				statusCtrl->fail(TASK_ERROR_INIT_DEVICE);
				PLOG(PLOG_ERROR, L"磁盘初始化错误");
				return false;
			}
			//创建缓存文件
			if (cloneCache->init() &&
				cacheImpl->oopen(param, hasher)) {
			}
			else {
				statusCtrl->fail();
				return false;
			}
		}
		cacheImpl->setProgress(statusCtrl->progress());
		cloneCache->start();
		return true;
	}

	void CPDeviceCloneTask::doTask(CPTask* task)
	{
		CPTaskStatusCtrl* statusCtrl = task->statusCtrl();
		//获取任务缓存
		CPCloneTaskCache* cloneCache = dynamic_cast<CPCloneTaskCache*>(task->getCache().get());
		if (!cloneCache) {
			statusCtrl->fail();
			return;
		}

		DeviceCloneParam param;
		CPHashComputerPtr hasher;
		TDeviceParam writeDeviceParam;
		CPDeviceWriterPtr writer;
		if (!init(task, cloneCache, param, hasher, writeDeviceParam, writer)) {
			statusCtrl->logInfo(PLOG_ERROR, L"初始化错误");
			return;
		}

		TDeviceParam readDeviceParam;
		CPDeviceTaskUtils::getDeviceParam(param.sourceDevice, readDeviceParam);

		CPDeviceReaderPtr reader = std::make_shared<CPDeviceReader>();
		if (!reader->open(readDeviceParam.diskPath)) {
			statusCtrl->fail(TASK_ERROR_OPEN_DEVICE);
			PLOG(PLOG_ERROR, L"打开磁盘错误");
			return;
		}

		CPRecord* record = task->rootRecord();
		record->at(L"source_size") = readDeviceParam.deviceSize;

		uint64 totalWriteSize = 0;
		if (param.eraseTail) {
			totalWriteSize = writeDeviceParam.deviceSize;
		}
		else {
			totalWriteSize = readDeviceParam.deviceSize;
		}
		statusCtrl->progress()->setTotalSize(totalWriteSize);

		//复制
		size_t startBlock = hasher->nextBlockNum();
		uint64 totalWrited = (uint64)startBlock * BLOCK_SIZE;
		if (totalWrited < readDeviceParam.deviceSize) {
			CPDeviceTaskCommon::doRwhWork(reader, readDeviceParam, writer,
				writeDeviceParam.deviceDisplayName, BLOCK_SIZE, startBlock, hasher, task);

			record->at(L"current_size") = statusCtrl->progress()->currentSize();
			if (hasher && !statusCtrl->isCanceled()) {
				std::wstring strHash;
				std::map<siio::HashType, std::string> hashValues;
				hasher->getValues(hashValues);
				CPDeviceLogUtils::getHashStr(hashValues, strHash, L";\n");
				record->at(L"hash") = strHash;
			}
		}

		if (param.eraseTail) {
			//擦除尾部
			CPDeviceTaskCommon::EraseOffset eraseOffset;
			eraseOffset.startOffset = readDeviceParam.deviceSize;
			eraseOffset.endOffset = writeDeviceParam.deviceSize;

			CPDataSourcePtr dataSource = std::make_shared<CPDataSource>(CPDataSource::DT_FIXED, COMMON_BUF_LEN);
			dataSource->setFixedChar(0);

            std::atomic<uint64> totalWrited(readDeviceParam.deviceSize);
			std::thread t(std::bind(&CPDeviceTaskCommon::eraseDevice,
				writeDeviceParam.diskPath, eraseOffset, dataSource, statusCtrl, totalWriteSize, &totalWrited));
			t.join();
		}

		cloneCache->clear();

		if (!statusCtrl->isCanceled() && !statusCtrl->isFailed()) {
			DeviceDisk targetDisk;
			bool ret = CPDeviceManager::get()->getDiskByName(param.targetDevice.deviceName, targetDisk);
#ifdef OS_PWINDOWS
			/*使磁盘脱机*/
			CPDiskpartTool::setDiskOnline(targetDisk.index, false);
#else
			CPDiskHandler::setDiskOnline(targetDisk.name, false);
#endif
		}
	}
}
