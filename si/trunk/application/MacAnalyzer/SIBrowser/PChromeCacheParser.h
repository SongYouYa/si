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
        @brief        解析chrome cache的index文件
        @param        [输入、输出]：    vEntryStores：    index中解析出的所有entryStore
        @param        [输入]：    version：            当前解析的版本
        @param        [输入]：    pProgressCtrl：    进度条
        @return        bool：        正常返回true，否则返回false
        @author        王虎
        @date        2011.10.10
        @remark
        *///}========================================================
        bool ParseChromeCacheIndex(std::vector<EntryStroreLoc>& vEntryStores, uint32* version);

        /*{=====================================================*//**
        @brief        解析entryStore
        @param        [输入]：    vEntryStores：    待解析出的所有entryStore
        @param        [输入]：    pProgressCtrl：    进度条
        @return        void
        @author        王虎
        @date        2011.10.10
        @remark
        *///}========================================================
        void ParseChromeCacheEntryStore(const EntryStroreLoc& vEntryStores);

        /*{=====================================================*//**
        @brief        解析单条 entry
        @param        [输入]：    pData：    指向待解析的 entry 起始位置
        @param        [输入]：    nBufSize：    有效待解析buf的长度
        @return        void
        @author        王虎
        @date        2011.10.10
        @remark
        *///}========================================================
        void ParseChromeCacheEntry( byte* pData, uint64 nBufSize );

        /*{=====================================================*//**
        @brief        解析单条 entry 的下一条 entry
        @param        [输入]：    entry：    待解析的 entry
        @return        void
        @author        王虎
        @date        2011.10.10
        @remark        并是不所有的entry都会有 next entry
        *///}========================================================
        void ParseEntryNext( const EntryStore& entry );

        uint64 ReadIndexFile();
        bool ReadData_0File();
        bool ReadData_1File();
        bool ReadData_2File();
        bool ReadData_3File();
        bool ReadData_4File();

        /*{=====================================================*//**
        @brief        解析entry的key
        @param        [输入]：    entry：        待解析的 entry
        @param        [输入]：    bPegPos：    当前解析 entry 的起始位置
        @param        [输入]：    pWebRecord：    解析后的record
        @param        [输入]：    nBufSize：    有效待解析buf的长度
        @author        王虎
        @date        2011.10.10
        @remark
        *///}========================================================
        void ParseEntryKey( const EntryStore& entry, byte* bPegPos, BrowserCache& oneCache, uint64 nBufSize );

        /*{=====================================================*//**
        @brief        解析entry的long key
        @param        [输入]：    entry：        待解析的 entry
        @param        [输入]：    pWebRecord：    解析后的record
        @return        void
        @author        王虎
        @date        2011.10.10
        @remark        只有少数的entry的key是该种情况
        *///}========================================================
        void ParseEntryLongKey( const EntryStore& entry, BrowserCache& oneCache);

        /*{=====================================================*//**
        @brief        解析entry的httpheader
        @param        [输入]：    entry：        待解析的 entry
        @param        [输入]：    pWebRecord：    解析后添加到的record
        @return        void
        @author        王虎
        @date        2011.10.10
        @remark
        *///}========================================================
//        void PsrseEntryHttpHeader( const EntryStore& entry, CPWebRecord* pWebRecord );

        /*{=====================================================*//**
        @brief        解析entry的真实缓存的数据内容
        @param        [输入]：    entry：        待解析的 entry
        @param        [输入]：    pWebRecord：    解析后添加到的record
        @return        void
        @author        王虎
        @date        2011.10.10
        @remark        只有少数的entry的key是该种情况
        *///}========================================================
//        void ParseEntryActualData( const EntryStore& entry, CPWebRecord* pWebRecord );

        /*{=====================================================*//**
        @brief        解析entry的rankingsNode
        @param        [输入]：    entry：        待解析的 entry
        @param        [输入]：    pWebRecord：    解析后添加到的record
        @return        void
        @author        王虎
        @date        2011.10.10
        @remark        主要是为了获得该entry的othertime和accesstime
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
        PSIHANDLE m_pFloder; //cache文件夹目录
		BrowserCaches Caches_;
    };
}

