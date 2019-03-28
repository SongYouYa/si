#pragma once


class CPFilePathChecker : public QObject
{
	Q_OBJECT
public:
	//���Ŀ��·���ʹ�С
	static bool checkTargetPath(const std::wstring& imagePath, 
		uint64 totalSize, uint64 singleSize);
	//��ȡ·�����ڵĴ���
	static bool getPathDevice(const std::wstring& filePath, 
		libdt::OneDevices& targetDevices);

private:
	//���Ŀ��ʣ��ռ��С
	static bool checkTargetFreeSpace(const std::wstring& targetPath, uint64 sourceSize);

	//����ļ�ϵͳ���ļ���С��FAT32���ܴ���4G��
    static bool checkTargetFsSize(const std::wstring& filesystem, uint64 fileSize);
};
