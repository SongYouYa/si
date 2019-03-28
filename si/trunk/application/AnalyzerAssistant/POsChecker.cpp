#include "stdafx.h"
#include "POsChecker.h"

#include "sidevice/PDeviceManager.h"
#include "utility/PSystemInfo.h"

//#include "DeviceEnumrator/PMounter.h"
#include "siio/PDeviceReader.h"

using namespace pdevice;

namespace app
{

#if 0
	std::vector<std::wstring> g_winFiles = {
		L"pagefile.sys", L"Windows/System32/ntoskrnl.exe",
		L"Windows/System32/ntdll.dll", L"Windows/System32/config/system",
		L"Windows/System32/config/software", L"Windows/System32/config/SAM",
		L"Program Files/"
	};

	std::vector<std::wstring> g_macFiles = {
		L"System/Library/Coreservices/SystemVersion.plist"
		L"Applications/App Store.app/Contents/MacOS/App Store",
		L"Applications/Safari.app/Contents/MacOS/Safari",
		L"dev/zero", L"dev/random",
		L"Volumes/"
	};

	typedef boost::filesystem::directory_iterator bfdit;

	bool isPathExists(const std::wstring& srcDir, const std::wstring& checkPath)
	{
		if (checkPath.empty()) {
			return true;
		}
		std::wstring fileName;
		std::wstring leftPath;
		std::string::size_type pos = checkPath.find(L"/");
		if (pos == std::string::npos) {
			fileName = checkPath;
		}
		else {
			fileName = checkPath.substr(0, pos);
			leftPath = checkPath.substr(pos + 1);
		}

		std::wstring findedDir;
		boost::filesystem::path p(srcDir);
		boost::filesystem::directory_iterator end;
		boost::filesystem::directory_iterator it(p);
		for (; it != end; ++it) {
			std::wstring itName = it->path().filename().wstring();
			if (CPString::isEquali(itName, fileName)) {
				findedDir = itName;
				break;
			}
		}
		if (findedDir.empty()) {
			return false;
		}
		return isPathExists(srcDir + L"/" + findedDir, leftPath);
	}

	bool checkOsFiles(const std::wstring& rootDir, std::vector<std::wstring>& osFiles)
	{
		for (auto& osFile : osFiles) {
			if (!isPathExists(rootDir, osFile)) {
				return false;
			}
		}
		return true;
	}

	void CPOsChecker::getOses(OsInfos& osInfos)
	{
		DeviceDisks disks;
		CPDeviceManager::get()->getAllFixedDisks(disks);

#ifdef OS_PWINDOWS
		std::wstring osDrive = util::CPSysteminfo::getSystemDrive();
#endif

		for (auto& disk : disks) {
			for (auto& partition : disk.partitions) {
				for (auto& mountPoint : partition.mountPoints) {
#ifdef OS_PWINDOWS
					if (mountPoint.find(osDrive) == 0) {
						OsInfo osInfo;
						osInfo.osType = OS_WIN;
						osInfo.online = true;
						osInfos.push_back(osInfo);
						continue;
					} else
#endif
					if (checkOsFiles(mountPoint, g_winFiles)) {
						OsInfo osInfo;
						osInfo.osType = OS_WIN;
						osInfos.push_back(osInfo);
					}
				}
			}
		}
	}
#endif

	CPOsChecker::CPOsChecker()
		: _hasMac(false), _hasWin(false)
	{
		init();
	}

	using namespace siio;
	const uint64 OS_SIZE_MIN = 10ULL * 1024 * 1024 * 1024;
	const uint SectorSize = 512;

	bool isPartitionApfs(siio::CPDeviceReader& reader)
	{
		std::vector<char> vbuf(SectorSize);
		char* buf = vbuf.data();
		int ret = reader.read(buf, SectorSize, NULL);
		if (ret != CPFile::PERROR_SUCCESS) {
			return false;
		}
		//APFS:在分区开始的32字节处，magic为NXSB
		if (memcmp(buf + 32, "NXSB", 4) == 0) {
			return true;
		}
		return false;
	}

	void CPOsChecker::init()
	{
		DeviceDisks disks;
		CPDeviceManager::get()->getLocalDisks(disks);

		//先通过文件系统类型来确定操作系统类型，
		//如果本地磁盘中，有HFS+文件系统，则认为有MacOs，
		//如果有NTFS，则认为有WinOs
		for (auto& disk : disks) {
			for (auto& partition : disk.partitions) {
				if (partition.size <= OS_SIZE_MIN) {
					continue;
				}
				if (CPString::findNoCase(partition.filesystem, L"hfsplus")) {
					_hasMac = true;
				}
				else if (CPString::findNoCase(partition.filesystem, L"ntfs")) {
					_hasWin = true;
				}
			}
		}

		if (!_hasMac && !_hasWin) {
			//如果未识别到操作系统，则检测是否有APFS文件系统，来识别MacOs
			for (auto& disk : disks) {
				for (auto& partition : disk.partitions) {
					siio::CPDeviceReader reader;
					if (!reader.open(partition.path)) {
						continue;
					}
					if (isPartitionApfs(reader)) {
						_hasMac = true;
						break;
					}
				}
				if (_hasMac) {
					break;
				}
			}
		}
	}

	bool CPOsChecker::hasMacOs()
	{
		return _hasMac;
	}
	bool CPOsChecker::hasWinOs()
	{
		return _hasWin;
	}
}