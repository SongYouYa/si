#include "stdafx.h"
#include "filesystemshell/FileSystemShell.h"
#include "siutility/PFileOperate.h"
#include "siutility/PFileSiUtils.h"
#include "siio/PHash.h"
//#include "system/CommonFileWriter.h"
//#include "siutility/ConvertDateTimeOperator.h"
using namespace siio;
using namespace filesystem;
namespace siutil {

CPFileOperate::CPFileOperate(void)
{
}


CPFileOperate::~CPFileOperate(void)
{
}

std::string CPFileOperate::GetFileInfoLine( const FileItemInfo& itemInfo )
{
    std::wstring line;
    line += itemInfo.path_;
    if(!itemInfo.md5Hash_.empty())
    {
        line += L",  MD5: " + itemInfo.md5Hash_;
    }
    line += L",  创建时间: " + itemInfo.createTime_.ToString();
    line += L",  修改时间: " + itemInfo.modifyTime_.ToString();
    line += L",  访问时间: " + itemInfo.accessTime_.ToString();

    std::string strline;
    strline = CPCodeChanger::UnicodeToUTF8(line.c_str());
    strline.append("\r\n");
    return strline;
}

void CPFileOperate::ParseFileInfo( PSIHANDLE itemHandle, FileItemInfo& itemInfo, bool isCalcHash)
{
    SPFileItem fileItem;
    filesystem::CFileSystem::GetFileItemInfo(itemHandle, &fileItem);
    itemInfo.size_ = fileItem.nSize;
    itemInfo.path_ = CFileSystem::GetItemPath(itemHandle);
    //获取文件在mac系统下的路径
    std::wstring mountPath = CFileSystem::GetItemPath(itemHandle);
    PSIHANDLE pvol = itemHandle;
    while(CFileSystem::GetItemType(pvol) != ITEM_TYPE_VOLUME)
    {
        pvol = CFileSystem::GetItemParent(pvol);
    }
    std::wstring mountPoint = CFileSystem::GetItemPath(pvol);
    std::string::size_type pos = mountPath.find(mountPoint);
    if(pos != std::string::npos)
    {
        itemInfo.path_ = mountPath.substr(pos + mountPoint.size());
    }
    //itemInfo.name_ = fileItem.strName; //乱码
    itemInfo.name_ = CFileSystem::GetItemName(itemHandle);
    itemInfo.createTime_ = CFileSystem::GetItemCreateTime(itemHandle);
    itemInfo.accessTime_ = CFileSystem::GetItemLastAccessTime(itemHandle);
    itemInfo.modifyTime_ = CFileSystem::GetItemModfiyTime(itemHandle);
    if(isCalcHash)
    {
        itemInfo.md5Hash_ = siutil::CPSiFileUtils::calcHashValue(itemHandle);
    }
}

}
