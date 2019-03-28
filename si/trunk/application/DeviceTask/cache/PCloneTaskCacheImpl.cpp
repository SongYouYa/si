#include "stdafx.h"
#include "PCloneTaskCacheImpl.h"

#include "libdevtask/PDeviceTaskUtils.h"
#include "libdevtask/cache/PDeviceIdArchiver.h"
#include "libdevtask/cache/PHashArchiver.h"

#include "siio/PDeviceReader.h"
#include "sicommon/PUtils.h"

using namespace siio;
using namespace libdt;
using namespace pdevice;

namespace devtask
{
	const std::wstring INDEX_FILE = L"/index";
	const std::wstring PROGRESS_FILE = L"/taskprogress";

	const uint LastSectorSize = 1024;
	const uint GuidLen = 32;

	CPCloneTaskCacheImpl::CPCloneTaskCacheImpl(const std::wstring& cacheDir)
		: _cacheDir(cacheDir),
		_progress(nullptr)
	{

	}

	bool CPCloneTaskCacheImpl::oopen(const DeviceCloneParam& cloneParam,
		libdt::CPHashComputerPtr hasher)
	{
		std::wstring indexPath = _cacheDir + INDEX_FILE;
		siio::CPNormalFile out;
		if (!out.open(indexPath)) {
			return false;
		}

		_hasher = hasher;

		//记录源盘ID
		DeviceId sourceId;
		CPDeviceTaskUtils::getDeviceId(cloneParam.sourceDevice, sourceId);
		if (!CPDeviceIdArchiver::serialize(&out, sourceId)) {
			return false;
		}
		//记录目标盘ID
		DeviceId targetId;
		CPDeviceTaskUtils::getDeviceId(cloneParam.targetDevice, targetId);
		if (!CPDeviceIdArchiver::serialize(&out, targetId)) {
			return false;
		}
		//由于通过磁盘ID来识别磁盘有风险（可能会有同型号的磁盘的ID相同）
		//所以在在硬盘最后写入guid，续传时通过此guid来识别
		std::string guid;
		if (!writeTargetGuid(_writer, guid)) {
			return false;
		}
		//记录guid
		out.write(guid.data(), guid.size());

		if (!CPHashArchiver::serialize(&out, _cacheDir, 0, _hasher, _hashCaches)) {
			return false;
		}
		char isEraseTail = cloneParam.eraseTail ? 1 : 0;
		out.write(&isEraseTail, sizeof(char));
		out.close();

		_progressCache = std::make_shared<CPProgressCache>();
		if (!_progressCache->open(_cacheDir + PROGRESS_FILE)) {
			return false;
		}

		return true;
	}

	bool CPCloneTaskCacheImpl::writeTargetGuid(siio::CPDeviceWriterPtr writer, std::string& guid)
	{
		if (!writer) {
			return false;
		}		
		uint64 diskSize = writer->size();
		if (diskSize < LastSectorSize) {
			return false;
		}
		if (!writer->seek(diskSize - LastSectorSize, PSEEK_BEG)) {
			return false;
		}
		guid = W2U8(CPUtils::getGuid()).substr(0, GuidLen);
		std::vector<char> vbuf(LastSectorSize, 0);
		memcpy(vbuf.data(), guid.c_str(), guid.size());
		int ret = writer->write(vbuf.data(), vbuf.size());
		if (ret != CPFile::PERROR_SUCCESS) {
			return false;
		}
		return true;
	}

	bool CPCloneTaskCacheImpl::getDiskGuid(const DeviceDisk& disk, std::string& guid)
	{
		CPDeviceReader diskReader;
		if (!diskReader.open(disk.path)) {
			return false;
		}
		uint64 diskSize = diskReader.size();
		if (diskSize < LastSectorSize) {
			return false;
		}
		if (!diskReader.seek(diskSize - LastSectorSize, PSEEK_BEG)) {
			return false;
		}
		std::vector<char> vbuf(LastSectorSize, 0);
		int ret = diskReader.read(vbuf.data(), vbuf.size());
		if (ret != CPFile::PERROR_SUCCESS) {
			return false;
		}
		guid.resize(GuidLen);
		memcpy((char*)guid.data(), vbuf.data(), guid.size());
		diskReader.close();
		return true;
	}

	bool CPCloneTaskCacheImpl::getTargetDisk(const pdevice::DeviceId& targetId,
		const std::string& targetGuid, libdt::OneDevice& targetDisk)
	{
		LessDiskId lid;
		lid.model = targetId.diskId.model;
		lid.size = targetId.diskId.size;
		lid.sn = targetId.diskId.sn;
		DeviceDisks mayDisks;
		CPDeviceManager::get()->getDisksByLid(lid, mayDisks);

		for (auto& disk : mayDisks) {
			std::string diskGuid;
			if (getDiskGuid(disk, diskGuid) &&
				diskGuid == targetGuid) {
				targetDisk.deviceName = disk.name;
				targetDisk.isPartition = false;
				return true;
			}
		}
		return false;
	}

	bool CPCloneTaskCacheImpl::iopen()
	{
		if (!_cloneParam.sourceDevice.deviceName.empty()) {
			return true;
		}
		std::wstring indexPath = _cacheDir + INDEX_FILE;
		siio::CPNormalFile out;
		if (!out.open(indexPath)) {
			return false;
		}
		DeviceId sourceId;
		if (!CPDeviceIdArchiver::load(&out, sourceId)) {
			return false;
		}
		if (!CPDeviceTaskUtils::getOneDevice(sourceId, _cloneParam.sourceDevice)) {
			return false;
		}
		//不能通过targetId来直接找磁盘，
		//因为目标磁盘的头部hash在克隆开始后会变化
		DeviceId targetId;
		if (!CPDeviceIdArchiver::load(&out, targetId)) {
			return false;
		}
		std::string targetGuid(GuidLen, 0);
		out.read((char*)targetGuid.data(), targetGuid.size());
		//通过设备ID和guid来找到设备
		if (!getTargetDisk(targetId, targetGuid, _cloneParam.targetDevice)) {
			return false;
		}

		if (!CPHashArchiver::load(&out, _cacheDir, _hasher, _hashCaches)) {
			return false;
		}
		_hasher->getHashTypes(_cloneParam.hashTypes);
		out.read(&_cloneParam.eraseTail, sizeof(char));
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

	void CPCloneTaskCacheImpl::getProgressInfo(ProgressInfo& progressInfo)
	{
		if (_progressCache) {
			_progressCache->getLastProgress(progressInfo);
		}
	}

	void CPCloneTaskCacheImpl::update()
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

		//progress
		if (_progressCache && _progress) {
			ProgressInfo progressInfo;
			progressInfo.used_time = _progress->getUsedTime();
			_progressCache->addProgress(progressInfo);
		}
	}

	void CPCloneTaskCacheImpl::close()
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
