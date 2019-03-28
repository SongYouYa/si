#include "stdafx.h"
#include <cstdio>
#include <cassert>
#include <iconv.h>
#include "PAddressBookAnalyzer.h"
#include <QString>

namespace macbasic {
    CPAddressAnalyzer::CPAddressAnalyzer()
    {
        m_pDB = NULL;
        //m_itPos = m_vecList.begin();
    }


    CPAddressAnalyzer::~CPAddressAnalyzer()
    {
        try
        {
            Close();
        }
        catch (...)
        {
    #ifdef _DEBUG
            printf("Exception throw...\n");
    #endif
        }
    }

    /**
     * @brief	打开数据库文件
     * @param	[in]:	wstrDBName			//数据库的名字
     * @return	bool	true	访问成功	false	访问失败
     * @author	nct(xgs@pansafe.com)
     * @date	2009.04.20
     */
    bool CPAddressAnalyzer::Open(const std::wstring & wstrDBName)
    {
        try
        {
            Close();
        }
        catch (...)
        {
    #ifdef _DEBUG
            printf("Exception throw...\n");
    #endif
        }

        if (wstrDBName.empty())
        {
            return false;
        }

        int iResult = 0;
        char szDBName[256];
        wcstombs(szDBName, wstrDBName.c_str(), wstrDBName.length() + 1);
        iResult = sqlite3_open(szDBName, &m_pDB);
        if (iResult)
        {
            fprintf(stderr, "Open database failed:\t%s\n", sqlite3_errmsg(m_pDB));
            sqlite3_close(m_pDB);
            return false;
        }
        else
        {
    #ifdef _DEBUG
            printf("Open database successfully!\n");
    #endif
        }

        return true;
    }

    void CPAddressAnalyzer::Close()
    {
        if (NULL != m_pDB)
        {
            sqlite3_close(m_pDB);
        }
        else
        {
            return;
        }
        m_pDB = NULL;

        if (m_vecList.size() > 0)
        {
            for (std::vector<CPAddressStruct *>::iterator it = m_vecList.begin();
                it != m_vecList.end(); ++it)
            {
                delete *it;
                *it = NULL;
            }

            m_vecList.clear();
        }
    }

    void CPAddressAnalyzer::Analyzer()
    {
        // This function must to exec first
        AnalyzerRecord();

        AnalyzerEmail();

        AnalyzerIM();

        AnalyzerPhoneBook();

        AnalyzerNote();

        AnalyzerRelated();

        AnalyzerPostedAddress();
    }

    const CPAddressStruct * CPAddressAnalyzer::GetFirstRecord()
    {
        if (m_vecList.size() == 0)
        {
            return NULL;
        }
        m_itPos = m_vecList.begin();
        return *m_itPos;
    }

    const CPAddressStruct * CPAddressAnalyzer::GetNextRecord()
    {
        ++m_itPos;
        if (m_vecList.end() == m_itPos)
        {
            return NULL;
        }
        else
        {
            return *m_itPos;
        }
    }

    void CPAddressAnalyzer::AnalyzerRecord(CPAddressStruct * pAddressInstance)
    {
        if (NULL == m_pDB)
        {
            return;
        }

        sqlite3_stmt *pStmt;
        if (SQLITE_OK != sqlite3_prepare(m_pDB, "select  Z_PK,ZUNIQUEID,ZFIRSTNAME,ZLASTNAME,ZORGANIZATION from ZABCDRECORD", -1, &pStmt, 0))
        {
            fprintf(stderr, "Queue record table failed:\t%s\n", sqlite3_errmsg(m_pDB));
            sqlite3_finalize(pStmt);
            return;
        }

        const int SIZE = 2048;
        char szData[SIZE] = {0};
        while(SQLITE_ROW == sqlite3_step(pStmt))
        {
            const wchar_t *pData = NULL;
            memset(szData, 0, SIZE);
            memcpy(szData, sqlite3_column_text16(pStmt, 1), sqlite3_column_bytes16(pStmt, 1));
            std::wstring wstrTmp = ucs2_ucs4(szData, sqlite3_column_bytes16(pStmt, 1));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                continue;
            }
            if (NULL == wcsstr(pData, L":ABPerson"))
            {
                continue;
            }

            int nID = sqlite3_column_int(pStmt, 0);

    #ifdef _DEBUG
            if (0 == nID)
            {
                printf("Addres ID:\t%d\n", nID);
            }
    #endif

            CPAddressStruct *pNewAddress = new CPAddressStruct(nID);
            if (NULL == pNewAddress)
            {
                fprintf(stderr, "Memory error!\n");
                continue;
            }

            memset(szData, 0, SIZE);
            //pData = (wchar_t *)sqlite3_column_text16(pStmt, 2);
            memcpy(szData, sqlite3_column_text16(pStmt, 2), sqlite3_column_bytes16(pStmt, 2));
            wstrTmp = ucs2_ucs4(szData, sqlite3_column_bytes16(pStmt, 2));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            pNewAddress->SetFirstName(std::wstring(pData));

            //pData = (wchar_t *)sqlite3_column_text16(pStmt, 3);
            wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt ,3), sqlite3_column_bytes16(pStmt, 3));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            pNewAddress->SetLastName(std::wstring(pData));

            //pData = (wchar_t *)sqlite3_column_text16(pStmt, 4);
            wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 4), sqlite3_column_bytes16(pStmt, 4));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            pNewAddress->SetCompanyName(std::wstring(pData));

            m_vecList.push_back(pNewAddress);
        }

        sqlite3_finalize(pStmt);
    }

    void CPAddressAnalyzer::AnalyzerEmail(CPAddressStruct * pAddressInstance)
    {
        if (NULL == m_pDB)
        {
            return;
        }

        sqlite3_stmt *pStmt = NULL;
        if (SQLITE_OK != sqlite3_prepare(m_pDB, "select  ZOWNER,ZADDRESS,ZLABEL from ZABCDEMAILADDRESS", -1, &pStmt, 0))
        {
            fprintf(stderr, "Queue email table failed:\t%s\n", sqlite3_errmsg(m_pDB));
            sqlite3_finalize(pStmt);
            return;
        }

        while(SQLITE_ROW == sqlite3_step(pStmt))
        {
            const wchar_t *pData = NULL;
            int nID = sqlite3_column_int(pStmt, 0);
            CPAddressStruct *pDest = NULL;
            for (std::vector<CPAddressStruct *>::iterator it = m_vecList.begin();
                it != m_vecList.end(); ++it)
            {
                if (NULL == *it)
                {
                    continue;
                }

                if ((*it)->GetID() == nID)
                {
                    pDest = *it;
                    break;
                }
            }

            //wchar_t *pData = (wchar_t *)sqlite3_column_text16(pStmt, 2);
            std::wstring wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 2), sqlite3_column_bytes16(pStmt, 2));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"_$!<Work>!$_";
            }
            ADDRTYPE mailType = AddrTypeIdentfy(std::wstring(pData));

            pData = (wchar_t *)sqlite3_column_text16(pStmt, 1);
            wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 2), sqlite3_column_bytes16(pStmt, 2));
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            //assert(NULL != pDest);
            if (NULL != pDest)
            {
                pDest->AddEmail(mailType, std::wstring(pData));
            }
            else
            {
              printf("Addressbook: dest is null\n");
            }

        }

        sqlite3_finalize(pStmt);
    }

    void CPAddressAnalyzer::AnalyzerIM(CPAddressStruct * pAddressInstance)
    {
        if (NULL == m_pDB)
        {
            return;
        }

        sqlite3_stmt *pStmt = NULL;
        if (SQLITE_OK != sqlite3_prepare(m_pDB, "select message.ZOWNER, message.ZADDRESS, message.ZLABEL, service.ZSERVICENAME from ZABCDMESSAGINGADDRESS message left join ZABCDSERVICE service on message.ZSERVICE = service.Z_PK", -1, &pStmt, 0))
        {
            fprintf(stderr, "Queue message table failed:\t%s\n", sqlite3_errmsg(m_pDB));
            sqlite3_finalize(pStmt);
            return;
        }

        while(SQLITE_ROW == sqlite3_step(pStmt))
        {
            const wchar_t * pData = NULL;
            int nID = sqlite3_column_int(pStmt, 0);
            CPAddressStruct *pDest = GetDestAddress(nID);
            if (NULL == pDest)
            {
                continue;
            }

            //wchar_t *pData = (wchar_t *)sqlite3_column_text16(pStmt, 2);
            std::wstring wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 2), sqlite3_column_bytes16(pStmt, 2));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"_$!<Work>!$_";
            }
            ADDRTYPE addrType = AddrTypeIdentfy(std::wstring(pData));

            //获取即时通信类型
            std::wstring wstrService = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 3), sqlite3_column_bytes16(pStmt, 3));

            //pData = (wchar_t *)sqlite3_column_text16(pStmt, 1);
            wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 1), sqlite3_column_bytes16(pStmt, 1));
            if (!wstrService.empty())
            {
                QString qstrService = QString::fromStdWString(wstrService);
                //去除类型字段中的Instant
                if (qstrService.contains(QString("Instant")))
                {
                    wstrService = qstrService.replace(QString("Instant"), QString("")).toStdWString();
                }
                wstrTmp = wstrService + L":" + wstrTmp;
            }

            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            //assert(NULL != pDest);
            if (NULL != pDest)
            {
              pDest->AddIM(addrType, std::wstring(pData));
            }
            else
            {
              printf("Addressbook: dest is null\n");
            }
        }

        sqlite3_finalize(pStmt);
    }

    void CPAddressAnalyzer::AnalyzerNote(CPAddressStruct * pAddressInstance)
    {
        if (NULL == m_pDB)
        {
            return;
        }

        sqlite3_stmt *pStmt = NULL;
        if (SQLITE_OK != sqlite3_prepare(m_pDB, "select ZCONTACT,ZTEXT from ZABCDNOTE", -1, &pStmt, 0))
        {
            fprintf(stderr, "Queue note table failed:\t%s\n", sqlite3_errmsg(m_pDB));
            sqlite3_finalize(pStmt);
            return;
        }

        while(SQLITE_ROW == sqlite3_step(pStmt))
        {
            const wchar_t *pData = NULL;
            int nID = sqlite3_column_int(pStmt, 0);
            CPAddressStruct *pDest = GetDestAddress(nID);
            if (NULL == pDest)
            {
                continue;
            }

            //wchar_t *pData = (wchar_t *)sqlite3_column_text16(pStmt, 1);
            std::wstring wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 1), sqlite3_column_bytes16(pStmt, 1));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            //assert(NULL != pDest);
            if (NULL != pDest)
            {
              pDest->SetNote(std::wstring(pData));
            }
            else
            {
              printf("Addressbook: dest is null\n");
            }
        }

        sqlite3_finalize(pStmt);
    }

    void CPAddressAnalyzer::AnalyzerPhoneBook(CPAddressStruct * pAddressInstance)
    {
        if (NULL == m_pDB)
        {
            return;
        }

        sqlite3_stmt *pStmt = NULL;
        if (SQLITE_OK != sqlite3_prepare(m_pDB, "select ZOWNER,ZFULLNUMBER,ZLABEL from ZABCDPHONENUMBER", -1, &pStmt, 0))
        {
            fprintf(stderr, "Queue phone book failed:\t%s\n", sqlite3_errmsg(m_pDB));
            sqlite3_finalize(pStmt);
            return;
        }

        while(SQLITE_ROW == sqlite3_step(pStmt))
        {
            const wchar_t *pData = NULL;
            int nID = sqlite3_column_int(pStmt, 0);
            CPAddressStruct *pDest = GetDestAddress(nID);
            if (NULL == pDest)
            {
                continue;
            }

            //wchar_t *pData = (wchar_t *)sqlite3_column_text16(pStmt, 2);
            std::wstring wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 2), sqlite3_column_bytes16(pStmt, 2));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"_$!<Work>!$_";
            }
            PHONETYPE phoneType = PhoneTypeIdentfy(std::wstring(pData));

            //pData = (wchar_t *)sqlite3_column_text16(pStmt, 1);
            wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 1), sqlite3_column_bytes16(pStmt, 1));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            //        assert(NULL != pDest);
            if (NULL != pDest)
            {
                pDest->AddPhone(phoneType, std::wstring(pData));
            }

            else
            {
              printf("Addressbook: dest is null\n");
            }
        }

        sqlite3_finalize(pStmt);
    }

    void CPAddressAnalyzer::AnalyzerRelated(CPAddressStruct * pAddressInstance)
    {
        if (NULL == m_pDB)
        {
            return;
        }

        sqlite3_stmt *pStmt = NULL;
        if (SQLITE_OK != sqlite3_prepare(m_pDB, "select ZOWNER,ZNAME from ZABCDRELATEDNAME", -1, &pStmt, 0))
        {
            fprintf(stderr, "Queue related failed:\t%s\n", sqlite3_errmsg(m_pDB));
            sqlite3_finalize(pStmt);
            return;
        }

        while(SQLITE_ROW == sqlite3_step(pStmt))
        {
            const wchar_t *pData = NULL;
            int nID = sqlite3_column_int(pStmt, 0);
            CPAddressStruct *pDest = GetDestAddress(nID);
            if (NULL == pDest)
            {
                continue;
            }

            //wchar_t *pData = (wchar_t *)sqlite3_column_text16(pStmt, 1);
            std::wstring wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 1), sqlite3_column_bytes16(pStmt, 1));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            //assert(NULL != pDest);
            if (NULL != pDest)
            {
                pDest->AddRelated(std::wstring(pData));
            }
            else
            {
              printf("Addressbook: dest is null\n");
            }
        }

        sqlite3_finalize(pStmt);
    }

    void CPAddressAnalyzer::AnalyzerPostedAddress(CPAddressStruct * pAddressInstance)
    {
        if (NULL == m_pDB)
        {
            return;
        }

        sqlite3_stmt *pStmt;
        if (SQLITE_OK != sqlite3_prepare(m_pDB, "select ZOWNER,ZCITY,ZLABEL,ZCOUNTRYNAME,ZSTREET,ZZIPCODE,ZSTATE from ZABCDPOSTALADDRESS", -1, &pStmt, 0))
        {
            fprintf(stderr, "Queue address failed:\t%s\n", sqlite3_errmsg(m_pDB));
            sqlite3_finalize(pStmt);
            return;
        }

        while(SQLITE_ROW == sqlite3_step(pStmt))
        {
            const wchar_t *pData = NULL;
            int nID = sqlite3_column_int(pStmt, 0);
            CPAddressStruct *pDest = GetDestAddress(nID);
            if (NULL == pDest)
            {
                continue;
            }

            ADDRESS newAddress;
            //wchar_t *pData = (wchar_t *)sqlite3_column_text16(pStmt, 1);
            std::wstring wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 1), sqlite3_column_bytes16(pStmt, 1));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            newAddress.m_wstrCity = pData;

            //pData = (wchar_t *)sqlite3_column_text16(pStmt, 3);
            wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 3), sqlite3_column_bytes16(pStmt, 3));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            newAddress.m_wstrCountry = pData;

            //pData = (wchar_t *)sqlite3_column_text16(pStmt, 4);
            wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 4), sqlite3_column_bytes16(pStmt, 4));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            newAddress.m_wstrStreet = pData;

            //pData = (wchar_t *)sqlite3_column_text16(pStmt, 5);
            wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 5), sqlite3_column_bytes16(pStmt, 5));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            newAddress.m_wstrZipCode = pData;

            //pData = (wchar_t *)sqlite3_column_text16(pStmt, 6);
            wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 6), sqlite3_column_bytes16(pStmt, 6));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            newAddress.m_wstrProvince = pData;

            //pData = (wchar_t *)sqlite3_column_text16(pStmt, 2);
            wstrTmp = ucs2_ucs4((char *)sqlite3_column_text16(pStmt, 2), sqlite3_column_bytes16(pStmt, 2));
            pData = const_cast<wchar_t *>(wstrTmp.c_str());
            if (NULL == pData)
            {
                pData = L"<null>";
            }
            ADDRTYPE addrType = AddrTypeIdentfy(std::wstring(pData));

            //assert(NULL != pDest);
            if (NULL != pDest)
            {
                pDest->AddAddress(addrType, newAddress);
            }
            else
            {
              printf("Addressbook: dest is null\n");
            }
        }

        sqlite3_finalize(pStmt);
    }

    ADDRTYPE AddrTypeIdentfy(const std::wstring & wstrIdentfy)
    {
        if (wstrIdentfy.find(L"Work") != std::wstring::npos)
        {
            return ADDR_TYPE_WORK;
        }
        if (wstrIdentfy.find(L"Home") != std::wstring::npos)
        {
            return ADDR_TYPE_HOME;
        }
        if (wstrIdentfy.find(L"Other") != std::wstring::npos)
        {
            return ADDR_TYPE_OTHER;
        }

        return ADDR_TYPE_DEFINE;
    }


    PHONETYPE PhoneTypeIdentfy(const std::wstring & wstrIdentfy)
    {
        if (wstrIdentfy.find(L"Work") != std::wstring::npos)
        {
            return PHONE_TYPE_WORK;
        }
        if (wstrIdentfy.find(L"Home") != std::wstring::npos)
        {
            return PHONE_TYPE_HOME;
        }
        if (wstrIdentfy.find(L"Main") != std::wstring::npos)
        {
            return PHONE_TYPE_MAIN;
        }
        if (wstrIdentfy.find(L"HomeFax") != std::wstring::npos)
        {
            return PHONE_TYPE_HOMEFAX;
        }
        if (wstrIdentfy.find(L"WorkFax") != std::wstring::npos)
        {
            return PHONE_TYPE_WORKFAX;
        }
        if (wstrIdentfy.find(L"Pager") != std::wstring::npos)
        {
            return PHONE_TYPE_CALL;
        }
        if (wstrIdentfy.find(L"Other") != std::wstring::npos)
        {
            return PHONE_TYPE_OTHER;
        }

        return PHONE_TYPE_DEFINE;
    }

    CPAddressStruct * CPAddressAnalyzer::GetDestAddress(int nID)
    {
        for (std::vector<CPAddressStruct *>::iterator it = m_vecList.begin();
            it != m_vecList.end(); ++it)
        {
            if (NULL != (*it) && (*it)->GetID() == nID)
            {
                return *it;
            }
        }

        return NULL;
    }


    std::wstring CPAddressAnalyzer::ucs2_ucs4(char * pointer, size_t size)
    {
        const int SIZE = 2048;
        char szData[SIZE] = {0};
        char * in = pointer;
        char * out = szData;
        size_t stIn = size;
        size_t stOut = SIZE;
    #if defined(__PPC__) || defined(__powerpc__)
        iconv_t cv = iconv_open("ucs-4be", "ucs-2be");
    #else
        iconv_t cv = iconv_open("ucs-4le", "ucs-2le");
    #endif
        if ((iconv_t)-1 == cv)
        {
            return L"";
        }
        iconv(cv, &in, &stIn, &out, &stOut);

        iconv_close(cv);

        return (wchar_t *)szData;
    }
}

