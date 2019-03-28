/*
 * =====================================================================================
 *
 *       Filename:  CAddressTask.cpp
 *
 *    Description:  AddressBook task class implement (.cpp file)
 *
 *        Version:  1.0
 *        Created:  2009年10月29日
 *       Revision:
 *
 *         Author:  Jesse.Xu (xgs), xgs@pansafe.com
 *        Company:  Pansafe Technology, INC
 *
 * =====================================================================================
 */
#include "stdafx.h"
#include <qstring.h>

#include "PAddressBookAnalyzer.h"
#include "PAddressTaskHelper.h"

#define ADDRESS_PATH L"/Library/Application Support/AddressBook"

using namespace siutil;
using namespace filesystem;
namespace macbasic {
    AddressTaskHelper::AddressTaskHelper()
    {


    }


    AddressTaskHelper::~AddressTaskHelper()
    {

    }

    bool AddressTaskHelper::GetAddressBookInformations(CPRecord* table)
    {

        std::wstring strRootPath;
        strRootPath = L"/Users";
        std::vector<std::wstring> vecFiles;
        CPMacFileExport fileExport(ADDRESS_PATH, L".abcddb");
        fileExport.GetExportFiles(vecFiles);
        //获取所有分区包含的用户
/*      std::set<std::wstring> setUsers;
        siutil::MacCommonHelper::GetMacAllUserName(setUsers);
        if (setUsers.empty())
        {
            return true;
        }
        std::vector<CFileExport*> exportFiles;
        //遍历获取所有数据文件
        for (std::set<std::wstring>::iterator iterUser = setUsers.begin();
              iterUser != setUsers.end(); ++iterUser)
        {
            if (pCtrl->IsCancel())
            {
                return false;
            }

            std::wstring strAnalysisPath = strRootPath + L"/" + *iterUser + ADDRESS_PATH;
            iterateAll(exportFiles, strAnalysisPath, vecFiles, pCtrl);
        }
        if (vecFiles.empty())
        {
            return true;
        }
*/

        for (std::vector<std::wstring>::iterator it = vecFiles.begin();
            it != vecFiles.end(); ++it)
        {

            if(!analysisAddress(*it, table))
            {
                continue;
            }
        }
        return true;
    }


    bool AddressTaskHelper::analysisAddress(const std::wstring & strAddress, CPRecord* table)
    {
        if (strAddress.empty())
        {
            return false;
        }

        CPAddressAnalyzer addressAnalyzer;
        bool bRet = addressAnalyzer.Open(strAddress);
        if (!bRet)
        {
            return false;
        }
        addressAnalyzer.Analyzer();
        return GetRecord(addressAnalyzer, table);
    }

    bool AddressTaskHelper::GetRecord(CPAddressAnalyzer & addressAnalyzer, CPRecord* table)
    {

        CPAddressStruct *pPrint = const_cast<CPAddressStruct *>(addressAnalyzer.GetFirstRecord());
        do
        {
            if (NULL == pPrint)
            {
                return false;
            }
            CPRecord *addressRecord = table->addChild();
            addressRecord->at(L"SIName") = pPrint->GetFirstName() + pPrint->GetLastName();
            addressRecord->at(L"SICompany") = pPrint->GetCompanyName();
            // Phone
            std::wstring wstrTmp;
            const std::multimap<PHONETYPE, std::wstring> & multimapPhone = pPrint->GetPhoneNumber();
            for (std::multimap<PHONETYPE, std::wstring>::const_iterator it = multimapPhone.begin();
                    it != multimapPhone.end(); ++it)
            {
                wstrTmp += it->second;
                wstrTmp += L";";
            }
            addressRecord->at(L"SIPhone") = wstrTmp;

            // Related
            wstrTmp.clear();
            const std::vector<std::wstring> & vecRelated = pPrint->GetRelated();
            for (std::vector<std::wstring>::const_iterator it = vecRelated.begin();
                    it != vecRelated.end(); ++it)
            {
                wstrTmp += *it;
                wstrTmp += L";";
            }
            addressRecord->at(L"SIRelated") = wstrTmp;

            // IM number
            wstrTmp.clear();
            const std::multimap<ADDRTYPE, std::wstring> & multimapIM = pPrint->GetIM();
            for (std::multimap<ADDRTYPE, std::wstring>::const_iterator it = multimapIM.begin();
                    it != multimapIM.end(); ++it)
            {
                wstrTmp += it->second;
                wstrTmp += L";";
            }
            addressRecord->at(L"SIIMNumber") = wstrTmp;
            // address
            wstrTmp.clear();
            const std::multimap<ADDRTYPE, ADDRESS> & multimapAddress = pPrint->GetAddress();
            for (std::multimap<ADDRTYPE, ADDRESS>::const_iterator it = multimapAddress.begin();
                    it != multimapAddress.end(); ++it)
            {
                wstrTmp = it->second.m_wstrCountry + it->second.m_wstrProvince
                    + it->second.m_wstrCity + it->second.m_wstrStreet + it->second.m_wstrZipCode;
                wstrTmp += L";";
            }

            addressRecord->at(L"SIAddress") = wstrTmp;
            addressRecord->at(L"SINote") = pPrint->GetNote();
        }
        while ((pPrint = const_cast<CPAddressStruct *>(addressAnalyzer.GetNextRecord())));
        return true;
    }


/*    bool AddressTaskHelper::iterateAll(std::vector<siutil::CFileExport* > &vecExportFiles, const std::wstring &strDir,
                                       std::vector<std::wstring> & vecFiles, task::TaskStatusCtrl *pCtrl)
    {
        std::vector<PSIHANDLE> pathHandles;
        CFileSystem::GetItemHandlesByRawPath(strDir, pathHandles);

        for (std::vector<PSIHANDLE>::iterator iter = pathHandles.begin();
             iter != pathHandles.end(); ++iter)
        {

            if (pCtrl->IsCancel())
            {
                return false;
            }
            CFileExport *pExportFile = new CFileExport;
            bool isFileExist = false;
            std::vector<PSIHANDLE> vecHandles;
            PSIHANDLE singleHandle = *iter;
            CFileSystem::EnumAllSubItems(singleHandle, vecHandles);
            for (size_t i=0; i<vecHandles.size(); ++i)
            {
                PSIHANDLE subHandle = vecHandles.at(i);
                SPFileItem fileItemInfo;
                CFileSystem::GetFileItemInfo(subHandle, &fileItemInfo);
                if (fileItemInfo.strName == L"." || fileItemInfo.strName == L".." || fileItemInfo.bFolder)
                {
                    continue;
                }
                else
                {
                    std::wstring strExt =  CFileSystem::GetFileExtName(subHandle);
                    if (strExt == L"abcddb")
                    {
                        std::wstring wstrFilePath = pExportFile->Export(subHandle);
                        vecFiles.push_back(wstrFilePath);
                        isFileExist = true;
                    }
                    else if (strExt == L"abcddb-shm")
                    {
                        pExportFile->Export(subHandle);
                        isFileExist = true;
                    }
                    else if (strExt == L"abcddb-wal")
                    {
                        pExportFile->Export(subHandle);
                        isFileExist = true;
                    }

                }
            }
            if (isFileExist)
            {
                 vecExportFiles.push_back(pExportFile);
            }

        }

        return true;
    }
*/
     bool AddressTaskHelper::GetAddressBookRecords(CPRecord* rootRecord)
     {   
        if (!GetAddressBookInformations(rootRecord))
        {
           return false;
        }
        return true;
     }

}
