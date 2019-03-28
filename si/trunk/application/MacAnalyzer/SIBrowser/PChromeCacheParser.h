#pragma once
#include <boost/scoped_array.hpp>
#include "PChromeCacheStruct.h"
#include "PBrowserInformation.h"

namespace macbasic {

    class CPChromeCacheParser
    {
    public:
        CPChromeCacheParser();
        ~CPChromeCacheParser(void);

        bool ParseFile(PSIHANDLE pFile);
		void GetCaches(BrowserCaches& caches);

    protected:
        /*{=====================================================*//**
        @brief        ����chrome cache��index�ļ�
        @param        [���롢���]��    vEntryStores��    index�н�����������entryStore
        @param        [����]��    version��            ��ǰ�����İ汾
        @param        [����]��    pProgressCtrl��    ������
        @return        bool��        ��������true�����򷵻�false
        @author        ����
        @date        2011.10.10
        @remark
        *///}========================================================
        bool ParseChromeCacheIndex(std::vector<EntryStroreLoc>& vEntryStores, uint32* version);

        /*{=====================================================*//**
        @brief        ����entryStore
        @param        [����]��    vEntryStores��    ��������������entryStore
        @param        [����]��    pProgressCtrl��    ������
        @return        void
        @author        ����
        @date        2011.10.10
        @remark
        *///}========================================================
        void ParseChromeCacheEntryStore(const EntryStroreLoc& vEntryStores);

        /*{=====================================================*//**
        @brief        �������� entry
        @param        [����]��    pData��    ָ��������� entry ��ʼλ��
        @param        [����]��    nBufSize��    ��Ч������buf�ĳ���
        @return        void
        @author        ����
        @date        2011.10.10
        @remark
        *///}========================================================
        void ParseChromeCacheEntry( byte* pData, uint64 nBufSize );

        /*{=====================================================*//**
        @brief        �������� entry ����һ�� entry
        @param        [����]��    entry��    �������� entry
        @return        void
        @author        ����
        @date        2011.10.10
        @remark        ���ǲ����е�entry������ next entry
        *///}========================================================
        void ParseEntryNext( const EntryStore& entry );

        uint64 ReadIndexFile();
        bool ReadData_0File();
        bool ReadData_1File();
        bool ReadData_2File();
        bool ReadData_3File();
        bool ReadData_4File();

        /*{=====================================================*//**
        @brief        ����entry��key
        @param        [����]��    entry��        �������� entry
        @param        [����]��    bPegPos��    ��ǰ���� entry ����ʼλ��
        @param        [����]��    pWebRecord��    �������record
        @param        [����]��    nBufSize��    ��Ч������buf�ĳ���
        @author        ����
        @date        2011.10.10
        @remark
        *///}========================================================
        void ParseEntryKey( const EntryStore& entry, byte* bPegPos, BrowserCache& oneCache, uint64 nBufSize );

        /*{=====================================================*//**
        @brief        ����entry��long key
        @param        [����]��    entry��        �������� entry
        @param        [����]��    pWebRecord��    �������record
        @return        void
        @author        ����
        @date        2011.10.10
        @remark        ֻ��������entry��key�Ǹ������
        *///}========================================================
        void ParseEntryLongKey( const EntryStore& entry, BrowserCache& oneCache);

        /*{=====================================================*//**
        @brief        ����entry��httpheader
        @param        [����]��    entry��        �������� entry
        @param        [����]��    pWebRecord��    ��������ӵ���record
        @return        void
        @author        ����
        @date        2011.10.10
        @remark
        *///}========================================================
//        void PsrseEntryHttpHeader( const EntryStore& entry, CPWebRecord* pWebRecord );

        /*{=====================================================*//**
        @brief        ����entry����ʵ�������������
        @param        [����]��    entry��        �������� entry
        @param        [����]��    pWebRecord��    ��������ӵ���record
        @return        void
        @author        ����
        @date        2011.10.10
        @remark        ֻ��������entry��key�Ǹ������
        *///}========================================================
//        void ParseEntryActualData( const EntryStore& entry, CPWebRecord* pWebRecord );

        /*{=====================================================*//**
        @brief        ����entry��rankingsNode
        @param        [����]��    entry��        �������� entry
        @param        [����]��    pWebRecord��    ��������ӵ���record
        @return        void
        @author        ����
        @date        2011.10.10
        @remark        ��Ҫ��Ϊ�˻�ø�entry��othertime��accesstime
        *///}========================================================
        void ParseRankingsNode( const EntryStore& entry, BrowserCache& oneCache );


    private:
        PSIHANDLE m_pIndexFile;
        PSIHANDLE m_pData_0File;
        PSIHANDLE m_pData_1File;
        PSIHANDLE m_pData_2File;
        PSIHANDLE m_pData_3File;
        PSIHANDLE m_pData_4File;

        byte* m_pIndexData;
        byte* m_pData_0Data;
        byte* m_pData_1Data;
        byte* m_pData_2Data;
        byte* m_pData_3Data;
        byte* m_pData_4Data;

        uint64 m_indexFileLength;
        uint64 m_data_0FileLength;
        uint64 m_data_1FileLength;
        uint64 m_data_2FileLength;
        uint64 m_data_3FileLength;
        uint64 m_data_4FileLength;

        byte* GetDataPointerByFileNum( uint32 uFileNum, size_t* nLength  );
        PSIHANDLE GetFilePointerByFileNum( uint32 uFileNum );
        uint64 ReadDataFileByFileName( const std::wstring& strFileName, PSIHANDLE& pFile, byte*& pData, uint64& fileLength );

	private:
        std::string m_TimezoneRegion;
        PSIHANDLE m_pFloder; //cache�ļ���Ŀ¼
		BrowserCaches Caches_;
    };
}

