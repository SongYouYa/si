// PInterfaceStruct.h
// 接口的一些结构
#ifndef PINTERFACESTRUCT_H
#define PINTERFACESTRUCT_H
#include <memory.h>

#ifdef __GNUC__
#define  __int64 long long
#endif
#pragma pack(1)

//设备
typedef struct tagDevice
{
    wchar_t				strDeviceName[256];		// 设备名
    wchar_t				strManufacture[256];	// 制造厂商
    wchar_t				strDeviceDesc[256];		// 描述
    wchar_t				strBusType[256];		// 总线类型
    unsigned __int64	nDeviceSize;			// 设备大小
    tagDevice()
    {
        memset(this, 0, sizeof(*this));
    }
} SPDevice;
// 卷
typedef struct tagVolume
{
    wchar_t				strVolumeName[256];		// 卷名
    unsigned __int64	nVolumeSize;			// 卷大小
    tagVolume()
    {
        memset(this, 0, sizeof(*this));
    }
} SPVolume;
// 时间
typedef struct tagDataTime
{
    unsigned short		year;
    unsigned char		month;
    unsigned char		day;
    unsigned char		hour;
    unsigned char		minute;
    unsigned char		second;
    char				timezone;
    tagDataTime()
    {
        memset(this, 0, sizeof(*this));
        timezone = 8;
    }
} SPDataTime;
// 文件项
typedef struct tagFileItem
{
    wchar_t				strName[256];			// 名称
    bool				bDeleted;				// 是否删除
    bool				bFolder;				// 是否为目录
    unsigned __int64	nSize;					// 文件大小，目录项为0
    unsigned __int64	nPhySize;				// 文件物理大小，目录项为0
    SPDataTime			tCreated;				// 创建日期
    SPDataTime			tAccessed;				// 访问日期
    SPDataTime			tModified;				// 修改日期
    tagFileItem()
    {
        memset(this, 0, sizeof(*this));
        tCreated.timezone = 8;
        tAccessed.timezone = 8;
        tModified.timezone = 8;
    }
} SPFileItem;
#pragma pack()
enum ITEM_CHECK_STATE
{
    ITEM_CHECK_NONE = 0,
    ITEM_CHECK_HALF,
    ITEM_CHECK_ALL,
};
// item 的类型
enum ITEM_TYPE
{
    ITEM_TYPE_NONE = 0, //错误类型
    ITEM_TYPE_DEVICE,   //设备
    ITEM_TYPE_VOLUME,   //卷
    ITEM_TYPE_DIRECTORTY, // 目录
    ITEM_TYPE_FILE     //文件
};

#endif
