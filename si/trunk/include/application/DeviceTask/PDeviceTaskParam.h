#pragma once

#include "DeviceEnumrator/PDeviceDef.h"
#include "sidevice/PDeviceDef.h"
#include "siio/PEnumDef.h"
#include "libdevtask/PDeviceTaskParams.h"

namespace devtask
{
	const uint BLOCK_SIZE = 16 * 1024 * 1024;

	//��¡
	struct DeviceCloneParam
	{
		libdt::OneDevice sourceDevice; //Դ���̣�Ӳ�̻������
		libdt::OneDevice targetDevice; //Ŀ�����
		std::vector<siio::HashType> hashTypes;
		bool eraseTail;
		DeviceCloneParam()
			: eraseTail(false)
		{}
	};

	struct ImageFormatOption
	{
		enum ImageType
		{
            IMAGE_NONE,
			IMAGE_DD,
			IMAGE_AFF,
            IMAGE_E01,
            IMAGE_ZIP   //�ļ����
		} imageType;
		uint64 splitSize;
		int compressLevel;
		ImageFormatOption()
            : imageType(IMAGE_NONE), splitSize(0), compressLevel(0)
		{}
	};

	struct ImageCommonParam
	{
		ImageFormatOption formatOptions; //ѡ�еľ����ʽ
		std::wstring imagePath; //����·��
		std::vector<siio::HashType> hashTypes;
	};
    //���̾���
	struct DeviceImageParam : public ImageCommonParam
	{
		libdt::OneDevice sourceDevice; //Դ���̣�Ӳ�̻������
#ifdef __APPLE__
        std::string volumePassword; //���ܾ������
#endif
	};

	//�ڴ澵��
	struct MemoryImageParam : public ImageCommonParam
	{
	};

	//��ʽ����������
	enum DeviceEraseType
	{
		ERASE_TYPE_SIMPLE, //�򵥲���
		ERASE_TYPE_BMB,
		ERADE_TYPE_DOD
	};
	struct DeviceEraseParam
	{
		DeviceEraseType eraseType;
		libdt::OneDevice targetDevice;
		DeviceEraseParam()
			: eraseType(ERASE_TYPE_SIMPLE)
		{}
	};

	//¼�����
	struct VideoParam
	{
		std::wstring  videoExePath;
		std::wstring  videoDirectorPath;
	};
	
	//�����ļ�����
	struct FileExportParam
	{
		CPRecord* folderRecord; //�ļ��������ڵ�
		bool withStructure; //�Ƿ񱣳ֽṹ
		std::wstring exportDir;
		std::vector<PSIHANDLE> rootHandles;
		bool exportFile; //�����ļ�����ֻ�����б�
		bool createZip; //�����ļ���ѹ����
		FileExportParam()
			: withStructure(true), exportFile(true), createZip(false)
		{}
	};
	enum FileExportState
	{
		FE_STATE_NONE,
		FE_STATE_HANDLE_PARSED	//��ѡ�ļ��Ѿ���ȡ����
	};
}
