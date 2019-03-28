#include "stdafx.h"
#include "application/DeviceTask/PDeviceImageTask.h"

#include "application/DeviceTask/cache/PImageTaskCache.h"
#include "./cache/PImageTaskCacheImpl.h"
#include "PFilePackageTask.h"

#include "libdevtask/PDeviceLogUtils.h"
#include "libdevtask/PDeviceTaskCommon.h"
#include "libdevtask/PDeviceTaskUtils.h"

#include "siio/PE01File.h"
#include "sifile/PAffFile.h"
#include "./material/PSplitDdFile.h"
#include "sifile/PMemoryReader.h"
#include "sicommon/PProcess.h"

using namespace pdevice;
using namespace siio;
using namespace libdt;

namespace devtask
{
	CPFilePtr getTargetFile(const ImageFormatOption& formatOptions)
	{
		CPFilePtr imageFile;
		if (formatOptions.imageType == ImageFormatOption::IMAGE_DD) {
			if (formatOptions.splitSize != 0) {
				imageFile = std::make_shared<CPSplitDdFile>(formatOptions.splitSize);
			}
			else {
				imageFile = std::make_shared<CPNormalFile>();
			}
		}
		else if (formatOptions.imageType == ImageFormatOption::IMAGE_E01) {
			imageFile = std::make_shared<CPE01File>(
				static_cast<CPE01File::COMPRESSION_LEVEL>(formatOptions.compressLevel));
		}
		else if (formatOptions.imageType == ImageFormatOption::IMAGE_AFF) {
			imageFile = std::make_shared<CPAffFile>(formatOptions.compressLevel);
		}
		return imageFile;
	}

	void doTaskImpl(CPTask* task,
		const ImageFormatOption& formatOptions,
		siio::CPFilePtr sourceFile,
		const TDeviceParam& sourceDevParam,
		const std::wstring& imagePath,
		CPHashComputerPtr hasher,
		CPFilePtr imageFile)
	{
		CPTaskStatusCtrl* statusCtrl = task->statusCtrl();
		std::wstring imageDir = CPPath::getDir(imagePath);
		if (!CPPath::createDirs(imageDir)) {
			statusCtrl->fail(TASK_ERROR_CREATE_DIR);
			return;
		}

		//在win7下，dd镜像采用flush清空缓存的方法，会有卡顿
		//所以采用不缓存的方式打开
		if (!imageFile->open(imagePath, CPFile::MODE_RW | CPFile::MODE_NOBUF)) {
			statusCtrl->fail(TASK_ERROR_CREATE_FILE);
			PLOG(PLOG_ERROR, L"打开镜像文件错误");
			return;
		}

		CPRecord* record = task->rootRecord();
		record->at(L"source_size") = sourceDevParam.deviceSize;

		statusCtrl->progress()->setTotalSize(sourceDevParam.deviceSize);

		size_t startBlock = hasher->nextBlockNum();
		CPDeviceTaskCommon::doRwhWork(sourceFile, sourceDevParam,
			imageFile, imagePath, BLOCK_SIZE, startBlock, hasher, task);

		record->at(L"current_size") = statusCtrl->progress()->currentSize();
		if (hasher && !statusCtrl->isCanceled()) {
			std::wstring strHash;
			std::map<siio::HashType, std::string> hashValues;
			hasher->getValues(hashValues);
			CPDeviceLogUtils::getHashStr(hashValues, strHash, L";\n");
			record->at(L"hash") = strHash;

			//新建一个txt文件，保持镜像的hash
			std::wstring txtPath = imagePath + L".txt";
			std::ofstream out(W2LS(txtPath));
			if (out.is_open()) {
				out << W2U8(task->getParamStr()) << std::endl;
				out << W2U8(strHash);
				out.close();
			}
		}
	}

#ifdef OS_PAPPLE
    //mac在线，对于加密卷，先解密后镜像或文件打包
    bool unlockVolume(const DeviceImageParam& param)
    {
        if (param.sourceDevice.isPartition) {
            DevicePartition sourcePartition;
            bool ret = CPDeviceManager::get()->getPartitionByName(
                        param.sourceDevice.deviceName, sourcePartition);
            assert(ret);
            if (sourcePartition.locked) {
                std::string cmd;
                if (sourcePartition.partitionType == PPARTITION_APFS_VOLUME) {
                    cmd = "/usr/sbin/diskutil apfs unlockVolume " +
                            W2LS(sourcePartition.path) + " -passphrase " + param.volumePassword;
                }
                else if (sourcePartition.partitionType == PPARTITION_HFSPLUS_VOLUME) {
                    cmd = "/usr/sbin/diskutil coreStorage unlockVolume " +
                            sourcePartition.uuid + " -passphrase " + param.volumePassword;
                }
                else {
                    return false;
                }
                int exitCode = 0;
                if (!CPProcess::createProcess(LS2W(cmd), L"", NULL, &exitCode) ||
                        exitCode != 0) {
                    return false;
                }
            }
        }
        return true;
    }
#endif

	bool init(CPTask* task, CPImageTaskCache* imageCache, DeviceImageParam& param,
		CPHashComputerPtr& hasher, CPFilePtr& imageFile)
	{
		CPTaskStatusCtrl* statusCtrl = task->statusCtrl();
		CPImageTaskCacheImpl* cacheImpl = imageCache->getImpl();
		bool isCached = imageCache->isInited(); //是否断点续传的任务
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
			param = boost::any_cast<DeviceImageParam>(*taskParamPtr);
			CPPath::formatPathSlash(param.imagePath);
			hasher = std::make_shared<CPHashComputer>(param.hashTypes);
#ifdef OS_PAPPLE
            if (!unlockVolume(param)) {
                statusCtrl->fail();
                return false;
            }
#endif
			//只有DD格式断点续传
			if (param.formatOptions.imageType == ImageFormatOption::IMAGE_DD) {
				//创建缓存文件
				if (imageCache->init() &&
					cacheImpl->oopen(param, hasher)) {
				}
				else {
					statusCtrl->fail();
					return false;
				}
			}
		}

		imageFile = getTargetFile(param.formatOptions);
		cacheImpl->setWriter(imageFile);

		cacheImpl->setProgress(statusCtrl->progress());
		if (param.formatOptions.imageType == ImageFormatOption::IMAGE_DD) {
			imageCache->start();
		}
		
		return true;
	}

	CPDeviceImageTask::CPDeviceImageTask()
	{

	}

	void CPDeviceImageTask::doDeviceTask(CPTask* task)
	{
		CPTaskStatusCtrl* statusCtrl = task->statusCtrl();
		//获取任务缓存
		CPImageTaskCache* imageCache = dynamic_cast<CPImageTaskCache*>(task->getCache().get());
		if (!imageCache) {
			statusCtrl->fail();
			return;
		}

		DeviceImageParam param;
		CPHashComputerPtr hasher;
		CPFilePtr imageFile;
		if (!init(task, imageCache, param, hasher, imageFile)) {
			statusCtrl->logInfo(PLOG_ERROR, L"初始化错误");
			return;
		}

        if (param.formatOptions.imageType == ImageFormatOption::IMAGE_ZIP) {
#ifdef OS_PAPPLE
            CPFilePackageTask::doPackage(param, task);
#else
			statusCtrl->fail();
			assert(false);
#endif
        }
        else {
            TDeviceParam sourceParam;
            CPDeviceTaskUtils::getDeviceParam(param.sourceDevice, sourceParam);
            CPDeviceReaderPtr reader = std::make_shared<CPDeviceReader>();
            if (!reader->open(sourceParam.diskPath)) {
                statusCtrl->fail(TASK_ERROR_PARAMERROR);
                PLOG(PLOG_ERROR, L"打开磁盘错误");
                return;
            }
            doTaskImpl(task, param.formatOptions, reader,
                sourceParam, param.imagePath, hasher, imageFile);
        }

		imageCache->clear();
	}

	void CPDeviceImageTask::doMemoryTask(CPTask* task)
	{
#if (defined OS_PWINDOWS) || (defined OS_PAPPLE)
		TaskParamPtr taskParamPtr = task->param();
		MemoryImageParam param = boost::any_cast<MemoryImageParam>(*taskParamPtr);
		CPTaskStatusCtrl* statusCtrl = task->statusCtrl();

		TDeviceParam sourceParam;

		CPMemoryReaderPtr memoryReader = std::make_shared<CPMemoryReader>();
		if (!memoryReader->open()) {
			statusCtrl->fail(TASK_ERROR_PARAMERROR);
			PLOG(PLOG_ERROR, L"打开内存错误");
			return;
		}
		sourceParam.deviceDisplayName = CPMemoryReader::getDisplayName();
		sourceParam.deviceSize = memoryReader->size();

		CPHashComputerPtr hasher = std::make_shared<CPHashComputer>(param.hashTypes);
		CPFilePtr imageFile = getTargetFile(param.formatOptions);
		doTaskImpl(task, param.formatOptions, memoryReader,
			sourceParam, param.imagePath, hasher, imageFile);
#endif
	}
}
