#include "stdafx.h"
#include "PDeviceLoader.h"
#include "filesystemshell/FileSystemShell.h"
#include "filesystemshell/PFsStatus.h"

using namespace filesystem;


#if defined(_DEBUG) && defined(OS_PWINDOWS)
#define FS_DEBUG 1
#endif


CPDeviceLoader* CPDeviceLoader::_instance = nullptr;

CPDeviceLoader::CPDeviceLoader()
: _finished(false),
_progress(0),
_timer(new QTimer(this))
{
	_timer->setInterval(1000);
	int unitp = 1000 / 60; //假设文件系统60秒加载完成
	connect(_timer, &QTimer::timeout, this, [this, unitp](){
		_progress += unitp;
		CPFsStatus::get()->setProgress(_progress);
	});
	connect(this, &CPDeviceLoader::signalLoadFinished, this, [this](){
		_timer->stop();
	}, Qt::QueuedConnection);
}

CPDeviceLoader* CPDeviceLoader::get()
{
	if (!_instance) {
		_instance = new CPDeviceLoader();
	}
	return _instance;
}

void CPDeviceLoader::getAllDeviceHandles(std::vector<PSIHANDLE>& handles)
{
	int diskCount = CFileSystem::LocalDeviceCount();
	for (int i = 0; i < diskCount; ++i) {
		PSIHANDLE deviceHandle = CFileSystem::GetDeviceInfo(i, nullptr);
		if (!CFileSystem::IsSafeImagerDisk(deviceHandle))
		{
			handles.push_back(deviceHandle);
		}
#ifdef FS_DEBUG
        handles.clear();
		if (0 == i) {
			handles.push_back(deviceHandle);
			break;
		}
#endif
	}

#ifndef FS_DEBUG
	int logicalVolumeCount = CFileSystem::LogicalVolumeCount();
	for (int i = 0; i < logicalVolumeCount; ++i)
	{
		PSIHANDLE volumeHandle = CFileSystem::GetLogicalVolumeInfo(i, nullptr);
		if (!CFileSystem::IsSafeImagerVolume(volumeHandle))
		{
			handles.push_back(volumeHandle);
		}
	}
#endif
}

void CPDeviceLoader::loadDevices(const std::vector<PSIHANDLE>& handles)
{
	_toloadDeviceHandles.clear();
	for (auto handle : handles) {
		bool loaded = false;
		for (auto loadedHandle : _loadedDeviceHandles) {
			if (loadedHandle == handle) {
				loaded = true;
				break;
			}
		}
		if (!loaded) {
			_toloadDeviceHandles.push_back(handle);
		}
	}
    start();
	_timer->start();
}

void CPDeviceLoader::loadAllDevices()
{
	std::vector<PSIHANDLE> handles;
	getAllDeviceHandles(handles);
	loadDevices(handles);
}

void CPDeviceLoader::run()
{
	{
		boost::mutex::scoped_lock lock(_mutex);
		_finished = false;
		emit signalLoadStart();
		
		bool ret = false;
		auto it = _toloadDeviceHandles.begin();
		for (; it != _toloadDeviceHandles.end(); ++it) {
			PSIHANDLE h = *it;
			bool loaded = false;
			for (auto it_l = _loadedDeviceHandles.begin(); it_l != _loadedDeviceHandles.end(); ++it_l) {
				if (h == *it_l) {
					loaded = true;
					break;
				}
			}
			if (!loaded) {
				ret |= CFileSystem::LoadDevice(h);
#if 0
				CFileSystem::SetItemChecked(h, true); //默认选中
#endif
				//逻辑卷的卷名需要加载后才有，所以在这里加载后再判断一次是否SI盘
				if (!CFileSystem::IsSafeImagerVolume(h)) {
					_loadedDeviceHandles.push_back(h);
				}
			}
		}
		_finished = true;
		CPFsStatus::get()->setLoaded();
		emit signalLoadFinished(ret);
	}
	_condtion.notify_all();
}

void CPDeviceLoader::waitForFinished()
{
	boost::mutex::scoped_lock lock(_mutex);
	if (!_finished)
	{
		_condtion.wait(_mutex);
	}
	return;
}

bool CPDeviceLoader::isFinished()
{
	return _finished;
}

void CPDeviceLoader::getLoadedDeviceHandles(std::vector<PSIHANDLE>& handles)
{
	handles = _loadedDeviceHandles;
}

void CPDeviceLoader::getCheckedDeviceHandles(std::vector<PSIHANDLE>& handles)
{
	for (auto it = _loadedDeviceHandles.begin(); it != _loadedDeviceHandles.end(); ++it) {
		ITEM_CHECK_STATE state = CFileSystem::ItemCheckState(*it);
		if (state != ITEM_CHECK_NONE) {
			handles.push_back(*it);
		}
	}
}
