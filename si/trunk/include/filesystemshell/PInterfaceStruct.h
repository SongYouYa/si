// PInterfaceStruct.h
// �ӿڵ�һЩ�ṹ
#ifndef PINTERFACESTRUCT_H
#define PINTERFACESTRUCT_H
#include <memory.h>

#ifdef __GNUC__
#define  __int64 long long
#endif
#pragma pack(1)

//�豸
typedef struct tagDevice
{
    wchar_t				strDeviceName[256];		// �豸��
    wchar_t				strManufacture[256];	// ���쳧��
    wchar_t				strDeviceDesc[256];		// ����
    wchar_t				strBusType[256];		// ��������
    unsigned __int64	nDeviceSize;			// �豸��С
    tagDevice()
    {
        memset(this, 0, sizeof(*this));
    }
} SPDevice;
// ��
typedef struct tagVolume
{
    wchar_t				strVolumeName[256];		// ����
    unsigned __int64	nVolumeSize;			// ���С
    tagVolume()
    {
        memset(this, 0, sizeof(*this));
    }
} SPVolume;
// ʱ��
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
// �ļ���
typedef struct tagFileItem
{
    wchar_t				strName[256];			// ����
    bool				bDeleted;				// �Ƿ�ɾ��
    bool				bFolder;				// �Ƿ�ΪĿ¼
    unsigned __int64	nSize;					// �ļ���С��Ŀ¼��Ϊ0
    unsigned __int64	nPhySize;				// �ļ������С��Ŀ¼��Ϊ0
    SPDataTime			tCreated;				// ��������
    SPDataTime			tAccessed;				// ��������
    SPDataTime			tModified;				// �޸�����
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
// item ������
enum ITEM_TYPE
{
    ITEM_TYPE_NONE = 0, //��������
    ITEM_TYPE_DEVICE,   //�豸
    ITEM_TYPE_VOLUME,   //��
    ITEM_TYPE_DIRECTORTY, // Ŀ¼
    ITEM_TYPE_FILE     //�ļ�
};

#endif
