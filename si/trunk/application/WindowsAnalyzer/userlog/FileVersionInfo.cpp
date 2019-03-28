#pragma once
#include "stdafx.h"
#include<windows.h>
#include "FileVersionInfo.h"

#pragma comment(lib,"version.lib")

CFileVersionInfo::CFileVersionInfo()
{
    Reset();
}
CFileVersionInfo::~CFileVersionInfo()
{}
bool CFileVersionInfo::GetTranslationId(LPVOID lpData, UINT unBlockSize, WORD wLangId, DWORD &dwId, bool bPrimaryEnough/*= false*/)
{
    LPWORD lpwData;
    
    for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData)+unBlockSize; lpwData+=2)
    {
        if (*lpwData == wLangId)
        {
            dwId = *((DWORD*)lpwData);
            return true;
        }
    }
    
    if (!bPrimaryEnough)
        return false;
        
    for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData)+unBlockSize; lpwData+=2)
    {
        if (((*lpwData)&0x00FF) == (wLangId&0x00FF))
        {
            dwId = *((DWORD*)lpwData);
            return true;
        }
    }
    
    return false;
}
//bool CFileVersionInfo::Create(HMODULE hModule /*= NULL*/)
//{
//	std::wstring	strPath;
//
//	GetModuleFileName(hModule, strPath.GetBuffer(_MAX_PATH), _MAX_PATH);
//	strPath.ReleaseBuffer();
//	return Create(strPath);
//}
bool CFileVersionInfo::Create(LPCTSTR lpszFileName)
{
    Reset();
    DWORD	dwHandle;
    DWORD	dwFileVersionInfoSize = GetFileVersionInfoSizeW((LPWSTR)lpszFileName, &dwHandle);
    
    if (!dwFileVersionInfoSize)
        return false;
        
    LPVOID	lpData = (LPVOID)new BYTE[dwFileVersionInfoSize];
    
    if (!lpData)
        return false;
        
    try
    {
        if (!GetFileVersionInfoW((LPWSTR)lpszFileName, dwHandle, dwFileVersionInfoSize, lpData))
            throw false;
            
        // catch default information
        LPVOID	lpInfo;
        UINT		unInfoLen;
        
        if (VerQueryValueW(lpData, L"\\", &lpInfo, &unInfoLen))
        {
            //ASSERT(unInfoLen == sizeof(m_FileInfo));
            if (unInfoLen == sizeof(m_FileInfo))
                memcpy(&m_FileInfo, lpInfo, unInfoLen);
        }
        
        // find best matching language and codepage
        VerQueryValueW(lpData, L"\\VarFileInfo\\Translation", &lpInfo, &unInfoLen);
        DWORD	dwLangCode = 0;
        
        if (!GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), dwLangCode, false))
        {
            if (!GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), dwLangCode, true))
            {
                if (!GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), dwLangCode, true))
                {
                    if (!GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL), dwLangCode, true))
                        // use the first one we can get
                        dwLangCode = *((DWORD*)lpInfo);
                }
            }
        }
        
        WCHAR SubBlock[MAX_PATH];
        swprintf(SubBlock,100, L"\\StringFileInfo\\%04X%04X\\", dwLangCode&0x0000FFFF, (dwLangCode&0xFFFF0000)>>16);
        std::wstring strSubBlock(SubBlock);
      //  strSubBlock.Format(L"\\StringFileInfo\\%04X%04X\\", dwLangCode&0x0000FFFF, (dwLangCode&0xFFFF0000)>>16);
        // catch string table
        std::wstring company_name = strSubBlock + L"CompanyName";
        
        if (VerQueryValueW(lpData, company_name.c_str(), &lpInfo, &unInfoLen))
        {
            m_strCompanyName = std::wstring((LPWSTR)lpInfo);
        }
        
        std::wstring file_description = strSubBlock+L"FileDescription";
        
        if (VerQueryValueW(lpData, file_description.c_str(), &lpInfo, &unInfoLen))
        {
            m_strFileDescription = std::wstring((LPWSTR)lpInfo);
        }
        
        std::wstring file_version =  strSubBlock+L"FileVersion";
        
        if (VerQueryValueW(lpData, file_version.c_str(), &lpInfo, &unInfoLen))
        {
            m_strFileVersion = std::wstring((LPWSTR)lpInfo);
        }
        
        std::wstring internal_name = strSubBlock+L"InternalName";
        
        if (VerQueryValueW(lpData, internal_name.c_str(), &lpInfo, &unInfoLen))
        {
            m_strInternalName = std::wstring((LPWSTR)lpInfo);
        }
        
        std::wstring legal_copyright = strSubBlock+L"LegalCopyright";
        
        if (VerQueryValueW(lpData, legal_copyright.c_str(), &lpInfo, &unInfoLen))
        {
            m_strLegalCopyright = std::wstring((LPWSTR)lpInfo);
        }
        
        std::wstring original_filename = strSubBlock+L"OriginalFileName";
        
        if (VerQueryValueW(lpData, original_filename.c_str(), &lpInfo, &unInfoLen))
        {
            m_strOriginalFileName = std::wstring((LPWSTR)lpInfo);
        }
        
        std::wstring product_name = strSubBlock+L"ProductName";
        
        if (VerQueryValueW(lpData, product_name.c_str(), &lpInfo, &unInfoLen))
        {
            m_strProductName = std::wstring((LPWSTR)lpInfo);
        }
        
        std::wstring product_version = strSubBlock+L"ProductVersion";
        
        if (VerQueryValueW(lpData,product_version.c_str() , &lpInfo, &unInfoLen))
        {
            m_strProductVersion = std::wstring((LPWSTR)lpInfo);
        }
        
        std::wstring comments = strSubBlock+L"Comments";
        
        if (VerQueryValueW(lpData, comments.c_str(), &lpInfo, &unInfoLen))
        {
            m_strComments = std::wstring((LPWSTR)lpInfo);
        }
        
        std::wstring legal_trade_marks  = strSubBlock+L"LegalTrademarks";
        
        if (VerQueryValueW(lpData, legal_trade_marks.c_str(), &lpInfo, &unInfoLen))
        {
            m_strLegalTrademarks = std::wstring((LPWSTR)lpInfo);
        }
        
        std::wstring private_build = strSubBlock+L"PrivateBuild";
        
        if (VerQueryValueW(lpData, private_build.c_str(), &lpInfo, &unInfoLen))
        {
            m_strPrivateBuild = std::wstring((LPWSTR)lpInfo);
        }
        
        std::wstring special_build = strSubBlock+L"SpecialBuild";
        
        if (VerQueryValueW(lpData, special_build.c_str(), &lpInfo, &unInfoLen))
        {
            m_strSpecialBuild = std::wstring((LPWSTR)lpInfo);
        }
        
        delete[] lpData;
    }
    catch (bool)
    {
        delete[] lpData;
        return false;
    }
    
    return true;
}
WORD CFileVersionInfo::GetFileVersion(int nIndex) const
{
    if (nIndex == 0)
        return (WORD)(m_FileInfo.dwFileVersionLS & 0x0000FFFF);
    else if (nIndex == 1)
        return (WORD)((m_FileInfo.dwFileVersionLS & 0xFFFF0000) >> 16);
    else if (nIndex == 2)
        return (WORD)(m_FileInfo.dwFileVersionMS & 0x0000FFFF);
    else if (nIndex == 3)
        return (WORD)((m_FileInfo.dwFileVersionMS & 0xFFFF0000) >> 16);
    else
        return 0;
}
WORD CFileVersionInfo::GetProductVersion(int nIndex) const
{
    if (nIndex == 0)
        return (WORD)(m_FileInfo.dwProductVersionLS & 0x0000FFFF);
    else if (nIndex == 1)
        return (WORD)((m_FileInfo.dwProductVersionLS & 0xFFFF0000) >> 16);
    else if (nIndex == 2)
        return (WORD)(m_FileInfo.dwProductVersionMS & 0x0000FFFF);
    else if (nIndex == 3)
        return (WORD)((m_FileInfo.dwProductVersionMS & 0xFFFF0000) >> 16);
    else
        return 0;
}
DWORD CFileVersionInfo::GetFileFlagsMask() const
{
    return m_FileInfo.dwFileFlagsMask;
}
DWORD CFileVersionInfo::GetFileFlags() const
{
    return m_FileInfo.dwFileFlags;
}
DWORD CFileVersionInfo::GetFileOs() const
{
    return m_FileInfo.dwFileOS;
}
DWORD CFileVersionInfo::GetFileType() const
{
    return m_FileInfo.dwFileType;
}
DWORD CFileVersionInfo::GetFileSubtype() const
{
    return m_FileInfo.dwFileSubtype;
}
//CTime CFileVersionInfo::GetFileDate() const
//{
//	FILETIME	ft;
//	ft.dwLowDateTime = m_FileInfo.dwFileDateLS;
//	ft.dwHighDateTime = m_FileInfo.dwFileDateMS;
//	return CTime(ft);
//}
std::wstring CFileVersionInfo::GetCompanyName() const
{
    return m_strCompanyName;
}
std::wstring CFileVersionInfo::GetFileDescription() const
{
    return m_strFileDescription;
}
std::wstring CFileVersionInfo::GetFileVersion() const
{
    return m_strFileVersion;
}
std::wstring CFileVersionInfo::GetInternalName() const
{
    return m_strInternalName;
}
std::wstring CFileVersionInfo::GetLegalCopyright() const
{
    return m_strLegalCopyright;
}
std::wstring CFileVersionInfo::GetOriginalFileName() const
{
    return m_strOriginalFileName;
}
std::wstring CFileVersionInfo::GetProductName() const
{
    return m_strProductName;
}
std::wstring CFileVersionInfo::GetProductVersion() const
{
    return m_strProductVersion;
}
std::wstring CFileVersionInfo::GetComments() const
{
    return m_strComments;
}
std::wstring CFileVersionInfo::GetLegalTrademarks() const
{
    return m_strLegalTrademarks;
}
std::wstring CFileVersionInfo::GetPrivateBuild() const
{
    return m_strPrivateBuild;
}
std::wstring CFileVersionInfo::GetSpecialBuild() const
{
    return m_strSpecialBuild;
}
void CFileVersionInfo::Reset()
{
    ZeroMemory(&m_FileInfo, sizeof(m_FileInfo));
    m_strCompanyName.clear();
    m_strFileDescription.clear();
    m_strFileVersion.clear();
    m_strInternalName.clear();
    m_strLegalCopyright.clear();
    m_strOriginalFileName.clear();
    m_strProductName.clear();
    m_strProductVersion.clear();
    m_strComments.clear();
    m_strLegalTrademarks.clear();
    m_strPrivateBuild.clear();
    m_strSpecialBuild.clear();
}
