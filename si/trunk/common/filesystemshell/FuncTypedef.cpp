#include "stdafx.h"
#include "FsDllLoader.h"
#include "FuncTypedef.h"

#define CheckFunc(x) if(x == NULL) {assert(x != NULL); return false;}

CFsDllLoader* gpFsDllLoader = NULL;
GETDEVICECOUNT* gpGetDeviceCountFunc = NULL;
GETDEVICE* gpGetDeviceFunc = NULL;
GETUSBDEVICECOUNT* gpGetUsbDeviceCountFunc = NULL;
GETUSBDEVICE* gpGetUsbDeviceFunc = NULL;
CREATEIMAGEDEVICE* pCreateImageDeviceFunc = NULL;
PARSEDEVICE* gpParseDeviceFunc = NULL;
GETVOLUMECOUNT* gpGetVolumeCountFunc = NULL;
RELEASEDEVICE* gpReleaseDeviceFunc = NULL;
RELEASEALLDEVICE* gpReleaseAllDeviceFunc = NULL;
GETVOLUME* gpGetVolumeFunc = NULL;
GETFILEITEMCOUNT* gpGetFileItemCountFunc = NULL;
GETFILEITEM* gpGetFileItemFunc = NULL;
GETFILEITEMINFO* gpGetFileItemInfoFunc = NULL;
READFILECONTENT* gpReadFileContentFunc = NULL;
GETITEMPARENT* gpGetItemParentFunc = NULL;
SETITEMCHECKSTATE* gpSetItemCheckStateFunc = NULL;
ITEMCHECKSTATE* gpItemCheckStateFunc = NULL;
GETITEMTYPE* gpGetItemTypeFunc = NULL;
GETITEMCREATETIME* gpGetItemCreateTimeFunc = NULL;
GETITEMMODIFYTIME* gpGetItemModifyTimeFunc = NULL;
GETITEMLASTACCESSTIME* gpGetItemLastAccessTimeFunc = NULL;
GETITEMNAME* gpGetItemNameFunc = NULL;
GETITEMSIZE* gpGetItemSizeFunc = NULL;
ISITEMDELETED* gpIsItemDeletedFunc = NULL;
GETLOGICALVOLUMECOUNT* gpGetLogicalVolumeCountFunc = NULL;
GETLOGICALVOLUME* gpGetLogicalVolumeFunc = NULL;
UPDATEDEVICETIMEZONE* gpUpdateDeviceTimeZoneFunc = NULL;
UPDATEVOLUMETIMEZONE* gpUpdateVolumeTimeZoneFunc = NULL;

CFsDllLoader* gMakeFsDllLoader(const std::wstring &dllDir)
{
    CFsDllLoader* pLoader = new CFsDllLoader();
    pLoader->setDllDir(dllDir);
    if(!pLoader->Load())
    {
        delete pLoader;
        return NULL;
    }
    
    return pLoader;
}
void gReleaseFsDllLoader()
{
    gpFsDllLoader->Release();
}
bool gLoadAllFunctions(const std::wstring& dllDir)
{
    if (!gpFsDllLoader)
    {
        gpFsDllLoader = gMakeFsDllLoader(dllDir);
        
        if (!gpFsDllLoader)
        {
            return false;
        }
    }
    
    gpGetDeviceCountFunc = (GETDEVICECOUNT*)gpFsDllLoader->GetFunction("GetDeviceCount");
    CheckFunc(gpGetDeviceCountFunc);

    gpGetDeviceFunc = (GETDEVICE*)gpFsDllLoader->GetFunction("GetDevice");
    CheckFunc(gpGetDeviceFunc);

    gpGetUsbDeviceCountFunc = (GETUSBDEVICECOUNT*)gpFsDllLoader->GetFunction("GetUSBDeviceCount");
    CheckFunc(gpGetUsbDeviceCountFunc);

    gpGetUsbDeviceFunc = (GETUSBDEVICE*)gpFsDllLoader->GetFunction("GetUSBDevice");
    CheckFunc(gpGetUsbDeviceFunc);

    gpParseDeviceFunc = (PARSEDEVICE*)gpFsDllLoader->GetFunction("ParseDevice");
    CheckFunc(gpParseDeviceFunc);

    gpReleaseDeviceFunc = (RELEASEDEVICE*)gpFsDllLoader->GetFunction("ReleaseDevice");
    CheckFunc(gpReleaseDeviceFunc);

    gpReleaseAllDeviceFunc = (RELEASEALLDEVICE*)gpFsDllLoader->GetFunction("ReleaseAllDevice");
    CheckFunc(gpReleaseAllDeviceFunc);

    gpGetVolumeCountFunc = (GETVOLUMECOUNT*)gpFsDllLoader->GetFunction("GetVolumeCount");
    CheckFunc(gpGetVolumeCountFunc);

    gpGetVolumeFunc = (GETVOLUME*)gpFsDllLoader->GetFunction("GetVolume");
    CheckFunc(gpGetVolumeFunc);

    gpGetFileItemCountFunc = (GETFILEITEMCOUNT*)gpFsDllLoader->GetFunction("GetFileItemCount");
    CheckFunc(gpGetFileItemCountFunc);

    gpGetFileItemFunc = (GETFILEITEM*)gpFsDllLoader->GetFunction("GetFileItem");
    CheckFunc(gpGetFileItemFunc);
    gpGetFileItemInfoFunc = (GETFILEITEMINFO*)gpFsDllLoader->GetFunction("GetFileItemInfo");
    CheckFunc(gpGetFileItemInfoFunc);

    gpReadFileContentFunc = (READFILECONTENT*)gpFsDllLoader->GetFunction("ReadFileContent");
    CheckFunc(gpReadFileContentFunc);

    gpItemCheckStateFunc = (ITEMCHECKSTATE*)gpFsDllLoader->GetFunction("ItemCheckState");
    CheckFunc(gpItemCheckStateFunc);

    gpSetItemCheckStateFunc = (SETITEMCHECKSTATE*)gpFsDllLoader->GetFunction("SetItemCheckState");
    CheckFunc(gpSetItemCheckStateFunc);

    gpGetItemParentFunc = (GETITEMPARENT*)gpFsDllLoader->GetFunction("GetItemParent");
    CheckFunc(gpGetItemParentFunc);
    gpGetItemTypeFunc = (GETITEMTYPE*)gpFsDllLoader->GetFunction("GetItemType");
    CheckFunc(gpGetItemTypeFunc);

    gpGetItemCreateTimeFunc = (GETITEMCREATETIME*)gpFsDllLoader->GetFunction("GetItemCreateTime");
    CheckFunc(gpGetItemCreateTimeFunc);

    gpGetItemModifyTimeFunc = (GETITEMMODIFYTIME*)gpFsDllLoader->GetFunction("GetItemModifyTime");
    CheckFunc(gpGetItemModifyTimeFunc);

    gpGetItemLastAccessTimeFunc = (GETITEMLASTACCESSTIME*)gpFsDllLoader->GetFunction("GetItemLastAccessTime");
    CheckFunc(gpGetItemLastAccessTimeFunc);

    gpGetItemNameFunc = (GETITEMNAME*)gpFsDllLoader->GetFunction("GetItemName");
    CheckFunc(gpGetItemNameFunc);

    gpGetItemSizeFunc = (GETITEMSIZE*)gpFsDllLoader->GetFunction("GetItemSize");
    CheckFunc(gpGetItemSizeFunc);

    gpIsItemDeletedFunc = (ISITEMDELETED*)gpFsDllLoader->GetFunction("IsItemDeleted");
    CheckFunc(gpIsItemDeletedFunc);

    gpGetLogicalVolumeCountFunc = (GETLOGICALVOLUMECOUNT*)gpFsDllLoader->GetFunction("GetLogicalVolumeCount");
    CheckFunc(gpGetLogicalVolumeCountFunc);
    
    gpGetLogicalVolumeFunc = (GETLOGICALVOLUME*)gpFsDllLoader->GetFunction("GetLogicalVolume");
    CheckFunc(gpGetLogicalVolumeFunc);

    gpUpdateDeviceTimeZoneFunc = (UPDATEDEVICETIMEZONE*)gpFsDllLoader->GetFunction("UpdateDeviceTimezone");
    CheckFunc(gpUpdateDeviceTimeZoneFunc);

    gpUpdateVolumeTimeZoneFunc = (UPDATEVOLUMETIMEZONE*)gpFsDllLoader->GetFunction("UpdateVolumeTimezone");
    CheckFunc(gpUpdateVolumeTimeZoneFunc);

    return true;
}
