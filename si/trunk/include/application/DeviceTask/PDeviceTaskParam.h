#pragma once

#include "DeviceEnumrator/PDeviceDef.h"
#include "sidevice/PDeviceDef.h"
#include "siio/PEnumDef.h"
#include "libdevtask/PDeviceTaskParams.h"

namespace devtask
{
	const uint BLOCK_SIZE = 16 * 1024 * 1024;

	//克隆
	struct DeviceCloneParam
	{
		libdt::OneDevice sourceDevice; //源磁盘（硬盘或分区）
		libdt::OneDevice targetDevice; //目标磁盘
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
            IMAGE_ZIP   //文件打包
		} imageType;
		uint64 splitSize;
		int compressLevel;
		ImageFormatOption()
            : imageType(IMAGE_NONE), splitSize(0), compressLevel(0)
		{}
	};

	struct ImageCommonParam
	{
		ImageFormatOption formatOptions; //选中的镜像格式
		std::wstring imagePath; //镜像路径
		std::vector<siio::HashType> hashTypes;
	};
    //磁盘镜像
	struct DeviceImageParam : public ImageCommonParam
	{
		libdt::OneDevice sourceDevice; //源磁盘（硬盘或分区）
#ifdef __APPLE__
        std::string volumePassword; //加密卷的密码
#endif
	};

	//内存镜像
	struct MemoryImageParam : public ImageCommonParam
	{
	};

	//格式化（擦除）
	enum DeviceEraseType
	{
		ERASE_TYPE_SIMPLE, //简单擦除
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

	//录像参数
	struct VideoParam
	{
		std::wstring  videoExePath;
		std::wstring  videoDirectorPath;
	};
	
	//磁盘文件导出
	struct FileExportParam
	{
		CPRecord* folderRecord; //文件导出父节点
		bool withStructure; //是否保持结构
		std::wstring exportDir;
		std::vector<PSIHANDLE> rootHandles;
		bool exportFile; //导出文件（否，只导出列表）
		bool createZip; //导出文件的压缩包
		FileExportParam()
			: withStructure(true), exportFile(true), createZip(false)
		{}
	};
	enum FileExportState
	{
		FE_STATE_NONE,
		FE_STATE_HANDLE_PARSED	//所选文件已经获取计算
	};
}
