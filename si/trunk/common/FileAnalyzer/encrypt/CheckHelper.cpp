#include "stdafx.h"
#include "CheckHelper.h"

namespace pfile {

CheckHelper::CheckHelper()
{

}

bool CheckHelper::CheckOLEHeader(PSIHANDLE pFile)
{
    if (NULL == pFile) {
        return false;
    }
#if _DEBUG
    std::wstring name = CFileSystem::GetItemName(pFile);
#endif
    if (CFileSystem::GetItemSize(pFile) < 1024) {
        return false;
    }
    byte header[8] = {0}; //first 8 bytes in *.doc|*.xls|*.ppt file
    byte magic[]= {0xd0,0xcf,0x11,0xe0,0xa1,0xb1,0x1a,0xe1};
    if (CFileSystem::ReadFile(pFile,0,8,header)){
        return memcmp(header,magic,sizeof(magic)) == 0;
    }
    return false;
}

bool CheckHelper::IsEncrypted(PSIHANDLE pFile)
{
    std::wstring extName = CFileSystem::GetFileExtName(pFile);
    bool bEncrypted = false;
    if(CPString::isEquali(extName, L"doc") && CheckOLEHeader(pFile))
    {
        bEncrypted = IsEncrypted_doc(pFile);
    }
	else if (CPString::isEquali(extName, L"docx"))
    {
        bEncrypted = IsEncrypted_docx(pFile);
    }
	else if (CPString::isEquali(extName, L"ppt") && CheckOLEHeader(pFile))
    {
        bEncrypted = IsEncrypted_ppt(pFile);
    }
	else if (CPString::isEquali(extName, L"pptx"))
    {
        bEncrypted = IsEncrypted_pptx(pFile);
    }
	else if (CPString::isEquali(extName, L"xls") && CheckOLEHeader(pFile))
    {
        bEncrypted = IsEncrypted_xls(pFile);
    }
	else if (CPString::isEquali(extName, L"xlsx"))
    {
        bEncrypted = IsEncrypted_xlsx(pFile);
    }
	else if (CPString::isEquali(extName, L"dmg"))
    {
        bEncrypted = IsEncrypted_dmg(pFile);
    }

    return bEncrypted;
}

}
