#ifndef SI_FILESYSTEM_FUNCTYPEDEF_H
#define SI_FILESYSTEM_FUNCTYPEDEF_H

//GetDeviceCount
typedef int (GETDEVICECOUNT)();
extern GETDEVICECOUNT* gpGetDeviceCountFunc;

//GetDevice
typedef PSIHANDLE (GETDEVICE)(unsigned int, SPDevice*);
extern GETDEVICE* gpGetDeviceFunc ;

//GetUSBDeviceCount
typedef int (GETUSBDEVICECOUNT)();
extern GETUSBDEVICECOUNT* gpGetUsbDeviceCountFunc;

//GetUSBFDevice
typedef PSIHANDLE (GETUSBDEVICE)(unsigned int, SPDevice*);
extern GETUSBDEVICE* gpGetUsbDeviceFunc;

//CreateImageDevice
typedef PSIHANDLE (CREATEIMAGEDEVICE)(const wchar_t*, SPDevice*);
extern CREATEIMAGEDEVICE* pCreateImageDeviceFunc;

//ParseDevice
typedef bool (PARSEDEVICE)(PSIHANDLE);
extern PARSEDEVICE* gpParseDeviceFunc;

// GetVolumeCount
typedef int (GETVOLUMECOUNT)(const PSIHANDLE);
extern GETVOLUMECOUNT* gpGetVolumeCountFunc;

//ReleaseDevice
typedef bool (RELEASEDEVICE)(PSIHANDLE);
extern RELEASEDEVICE* gpReleaseDeviceFunc;

// 找 ReleaseAllDevice
typedef void (RELEASEALLDEVICE)();
extern RELEASEALLDEVICE* gpReleaseAllDeviceFunc;

//GetVolume
typedef PSIHANDLE (GETVOLUME)(const PSIHANDLE, unsigned int, SPVolume*);
extern GETVOLUME* gpGetVolumeFunc;

//GetFileItemCount
typedef int (GETFILEITEMCOUNT)(const PSIHANDLE);
extern GETFILEITEMCOUNT* gpGetFileItemCountFunc;

//GetFileItem
typedef PSIHANDLE (GETFILEITEM)(const PSIHANDLE, unsigned int, SPFileItem*);
extern GETFILEITEM* gpGetFileItemFunc;

//GetFileItemInfo
typedef bool (GETFILEITEMINFO)( const PSIHANDLE , SPFileItem* );
extern GETFILEITEMINFO* gpGetFileItemInfoFunc;

//ReadFileContent
typedef unsigned __int64 (READFILECONTENT)(const PSIHANDLE, unsigned __int64 , unsigned __int64, unsigned char*);
extern READFILECONTENT* gpReadFileContentFunc;

//GetItemParent
typedef PSIHANDLE (GETITEMPARENT)(const PSIHANDLE);
extern GETITEMPARENT* gpGetItemParentFunc;

//SetItemCheckState
typedef void (SETITEMCHECKSTATE)(const PSIHANDLE, ITEM_CHECK_STATE);
extern SETITEMCHECKSTATE* gpSetItemCheckStateFunc;

//ItemCheckState
typedef ITEM_CHECK_STATE (ITEMCHECKSTATE)(const PSIHANDLE);
extern ITEMCHECKSTATE* gpItemCheckStateFunc;

//GetItemType
typedef ITEM_TYPE (GETITEMTYPE)(const PSIHANDLE);
extern GETITEMTYPE* gpGetItemTypeFunc;

typedef PPTIME(GETITEMCREATETIME)(const PSIHANDLE);
extern GETITEMCREATETIME* gpGetItemCreateTimeFunc;

typedef PPTIME(GETITEMMODIFYTIME)(const PSIHANDLE);
extern GETITEMMODIFYTIME* gpGetItemModifyTimeFunc;

typedef PPTIME(GETITEMLASTACCESSTIME)(const PSIHANDLE);
extern GETITEMLASTACCESSTIME* gpGetItemLastAccessTimeFunc;

typedef int (GETITEMNAME)(const PSIHANDLE, wchar_t** );
extern GETITEMNAME* gpGetItemNameFunc;

typedef unsigned __int64 (GETITEMSIZE)(const PSIHANDLE);
extern GETITEMSIZE* gpGetItemSizeFunc;

typedef bool (ISITEMDELETED)(const PSIHANDLE);
extern ISITEMDELETED* gpIsItemDeletedFunc;

typedef int (GETLOGICALVOLUMECOUNT)();
extern GETLOGICALVOLUMECOUNT* gpGetLogicalVolumeCountFunc;

typedef PSIHANDLE (GETLOGICALVOLUME)( unsigned int ,SPVolume* );
extern GETLOGICALVOLUME* gpGetLogicalVolumeFunc;


typedef bool (UPDATEDEVICETIMEZONE)(PSIHANDLE ,long);
extern UPDATEDEVICETIMEZONE* gpUpdateDeviceTimeZoneFunc;

typedef bool (UPDATEVOLUMETIMEZONE)(PSIHANDLE ,long);
extern UPDATEVOLUMETIMEZONE* gpUpdateVolumeTimeZoneFunc;

//Dll 加载相关的函数
class CFsDllLoader;
CFsDllLoader* gMakeFsDllLoader(const std::wstring& dllDir);
bool gLoadAllFunctions(const std::wstring& dllDir);
void gReleaseFsDllLoader();
#endif
