#include "stdafx.h"
#include "PImageTaskCacheImpl.h"

#include "libdevtask/PDeviceTaskUtils.h"
#include "libdevtask/cache/PDeviceIdArchiver.h"
#include "libdevtask/cache/PHashArchiver.h"

#include "siutility/PDevicePath.h"

using namespace siio;
using namespace libdt;
using namespace pdevice;

namespace devtask
{
	const std::wstring INDEX_FILE = L"/index";
	const std::wstring PROGRESS_FILE = L"/taskprogress";

	struct ImageCacheC
	{
		short relative_path_len;
		short image_type;
		short compress_level;
		uint64 split_size;
	};

    bool findMountPoint(const std::vector<std::wstring>& mountPoints,
                        const std::wstring& imagePath, std::wstring& mountPoint)
    {
        for (auto mp : mountPoints) {
            if (mp.empty()) {
                continue;
            }
            std::wstring tempMp = mp;
#ifdef OS_PUNIX
            if (tempMp.back() != L'/') {
                tempMp += L'/';
            }
#else
            if (tempMp.back() != L'\\') {
                tempMp += L'\\';
            }
#endif
            std::string::size_type pos = imagePath.find(tempMp);
            if (pos != std::string::npos) {
                mountPoint = mp;
                return true;
            }
        }
        return false;
    }

	CPImageTaskCacheImpl::CPImageTaskCacheImpl(const std::wstring& cacheDir)
		: _cacheDir(cacheDir),
		_progress(nullptr)
	{

	}

	bool CPImageTaskCacheImpl::oopen(const DeviceImageParam& imageParam, 
		libdt::CPHashComputerPtr hasher)
	{
		std::wstring indexPath = _cacheDir + INDEX_FILE;
		siio::CPNormalFile out;
		if (!out.open(indexPath)) {
			return false;
		}

		_hasher = hasher;

		//源盘
		DeviceId sourceId;
		CPDeviceTaskUtils::getDeviceId(imageParam.sourceDevice, sourceId);
		if (!CPDeviceIdArchiver::serialize(&out, sourceId)) {
			return false;
		}
		//目标路径所在磁盘
		DeviceDisk targetDisk;
		DevicePartition targetPartition;
		if (!siutil::CPDevicePath::getPathDevice(imageParam.imagePath,
			targetDisk, targetPartition)) {
			return false;
		}
		DeviceId targetId;
		targetId.deviceSize = targetPartition.size;
        if (!targetDisk.name.empty()) {
            CPDeviceManager::get()->getDiskId(targetDisk.name, targetId.diskId);
            if (!targetPartition.name.empty()) {
                targetId.isPartition = true;
                targetId.partitionIndex = targetPartition.index;
            }
            else {
                targetId.isPartition = false;
            }
        }
        else {
            targetId.isPartition = true;
            //目标路径是远程逻辑分区
            targetId.isLogicPartition = true;
            targetId.deviceName = targetPartition.name;
        }

		CPDeviceIdArchiver::serialize(&out, targetId);

		//保存目标路径在卷下的相对路径
        std::wstring mountPoint;
        if (!findMountPoint(targetPartition.mountPoints, imageParam.imagePath, mountPoint) &&
                !findMountPoint(targetDisk.mountPoints, imageParam.imagePath, mountPoint)) {
            return false;
        }
		std::string::size_type pos = imageParam.imagePath.find(mountPoint);
		if (pos == std::string::npos) {
			return false;
		}
		std::string relativePath = W2U8(imageParam.imagePath.substr(mountPoint.size()));

		ImageCacheC imageCacheC;
		memset(&imageCacheC, 0, sizeof(ImageCacheC));
		imageCacheC.relative_path_len = relativePath.size();
		imageCacheC.image_type = imageParam.formatOptions.imageType;
		imageCacheC.compress_level = imageParam.formatOptions.compressLevel;
		imageCacheC.split_size = imageParam.formatOptions.splitSize;
		out.write(&imageCacheC, sizeof(ImageCacheC));
		out.write(relativePath.data(), relativePath.size());
		
		if (!CPHashArchiver::serialize(&out, _cacheDir, 0, _hasher, _hashCaches)) {
			return false;
		}
		out.close();

		_progressCache = std::make_shared<CPProgressCache>();
		if (!_progressCache->open(_cacheDir + PROGRESS_FILE)) {
			return false;
		}

		return true;
	}

	bool CPImageTaskCacheImpl::iopen()
	{
		if (!_imageParam.sourceDevice.deviceName.empty()) {
			//已经打开了
			return true;
		}
		std::wstring indexPath = _cacheDir + INDEX_FILE;
		siio::CPNormalFile out;
		if (!out.open(indexPath)) {
			return false;
		}
		//源盘
		DeviceId sourceId;
		if (!CPDeviceIdArchiver::load(&out, sourceId)) {
			return false;
		}
		if (!CPDeviceTaskUtils::getOneDevice(sourceId, _imageParam.sourceDevice)) {
			return false;
		}
		//目标路径所在磁盘
		DeviceId targetId;
		if (!CPDeviceIdArchiver::load(&out, targetId)) {
			return false;
		}
		OneDevice targetDevice;
        if (!CPDeviceTaskUtils::getOneDevice(targetId, targetDevice) &&
                !CPDeviceTaskUtils::getOneDevice(targetId, targetDevice, true)) {
			return false;
		}

        std::wstring mountPoint;
        if (targetDevice.isPartition) {
            DevicePartition targetPartition;
            if (!CPDeviceManager::get()->getPartitionByName(targetDevice.deviceName, targetPartition) ||
                    targetPartition.mountPoints.empty()) {
               return false;
            }
            mountPoint = targetPartition.mountPoints.at(0);
        }
        else {
            DeviceDisk targetDisk;
            if (!CPDeviceManager::get()->getDiskByName(targetDevice.deviceName, targetDisk) ||
                    targetDisk.mountPoints.empty()) {
               return false;
            }
            mountPoint = targetDisk.mountPoints.at(0);
        }
        if (mountPoint.empty()) {
			return false;
		}

		ImageCacheC imageCacheC;
		memset(&imageCacheC, 0, sizeof(ImageCacheC));
		out.read(&imageCacheC, sizeof(ImageCacheC));
		_imageParam.formatOptions.imageType = static_cast<ImageFormatOption::ImageType>(imageCacheC.image_type);
		_imageParam.formatOptions.compressLevel = imageCacheC.compress_level;
		_imageParam.formatOptions.splitSize = imageCacheC.split_size;
		std::string relativePath(imageCacheC.relative_path_len, 0);
		out.read((char*)relativePath.data(), relativePath.size());
		_imageParam.imagePath = mountPoint + U82W(relativePath);

		if (!CPHashArchiver::load(&out, _cacheDir, _hasher, _hashCaches)) {
			return false;
		}
		_hasher->getHashTypes(_imageParam.hashTypes);
		out.close();

		std::wstring progressCachePath = _cacheDir + PROGRESS_FILE;
		if (!CPProgressCacheHandler::createCacheFromOld(progressCachePath)) {
			return false;
		}
		_progressCache = std::make_shared<CPProgressCache>();
		if (!_progressCache->open(progressCachePath)) {
			return false;
		}

		return true;
	}

	void CPImageTaskCacheImpl::getProgressInfo(ProgressInfo& progressInfo)
	{
		if (_progressCache) {
			_progressCache->getLastProgress(progressInfo);
		}
	}

	void CPImageTaskCacheImpl::update()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		if (!_writer) {
			return;
		}
		//先缓存writer
		if (!_writer->flush()) {
			return;
		}

		//后缓存hash
        if (_hasher) {
            TypeHashCtxs hashCtxs;
            _hasher->getHashCtxs(hashCtxs);

            for (auto& it_h : hashCtxs) {
                auto it_hc = _hashCaches.find(it_h.first);
                if (it_hc != _hashCaches.end()) {
                    HashCtx& ctx = it_h.second;
                    CPHashCachePtr& hashCache = it_hc->second;
                    hashCache->addCtx(ctx);
                }
            }
        }

		//progress
		if (_progressCache && _progress) {
			ProgressInfo progressInfo;
			progressInfo.used_time = _progress->getUsedTime();
			_progressCache->addProgress(progressInfo);
		}
	}

	void CPImageTaskCacheImpl::close()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		for (auto& it : _hashCaches) {
			CPHashCachePtr& hashCache = it.second;
			hashCache->close();
		}
		if (_progressCache) {
			_progressCache->close();
		}
	}
}
