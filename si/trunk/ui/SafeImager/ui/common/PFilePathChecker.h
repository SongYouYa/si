#pragma once


class CPFilePathChecker : public QObject
{
	Q_OBJECT
public:
	//检测目标路径和大小
	static bool checkTargetPath(const std::wstring& imagePath, 
		uint64 totalSize, uint64 singleSize);
	//获取路径所在的磁盘
	static bool getPathDevice(const std::wstring& filePath, 
		libdt::OneDevices& targetDevices);

private:
	//检测目标剩余空间大小
	static bool checkTargetFreeSpace(const std::wstring& targetPath, uint64 sourceSize);

	//检测文件系统和文件大小（FAT32不能大于4G）
    static bool checkTargetFsSize(const std::wstring& filesystem, uint64 fileSize);
};
