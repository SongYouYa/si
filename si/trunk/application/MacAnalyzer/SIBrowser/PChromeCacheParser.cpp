#include "stdafx.h"
#include "PChromeCacheParser.h"
#include <boost/scoped_array.hpp>
using namespace filesystem;

namespace macbasic{

    CPChromeCacheParser::CPChromeCacheParser(void)
    {
        m_pIndexFile = NULL;
        m_pData_0File = NULL;
        m_pData_1File = NULL;
        m_pData_2File = NULL;
        m_pData_3File = NULL;
        m_pData_4File = NULL;

        m_pIndexData = NULL;
        m_pData_0Data = NULL;
        m_pData_1Data = NULL;
        m_pData_2Data = NULL;
        m_pData_3Data = NULL;
        m_pData_4Data = NULL;

        m_indexFileLength = 0;
        m_data_0FileLength = 0;
        m_data_1FileLength = 0;
        m_data_2FileLength = 0;
        m_data_3FileLength = 0;
        m_data_4FileLength = 0;
    }

    CPChromeCacheParser::~CPChromeCacheParser(void)
    {
        if ( m_pIndexData )
        {
            delete[] m_pIndexData;
            m_pIndexData = NULL;
        }
        if ( m_pData_0Data )
        {
            delete[] m_pData_0Data;
            m_pData_0Data = NULL;
        }
        if ( m_pData_1Data )
        {
            delete[] m_pData_1Data;
            m_pData_1Data = NULL;
        }

        if ( m_pData_2Data )
        {
            delete[] m_pData_2Data;
            m_pData_2Data = NULL;
        }

        if ( m_pData_3Data )
        {
            delete[] m_pData_3Data;
            m_pData_3Data = NULL;
        }

        if ( m_pData_4Data )
        {
            delete[] m_pData_4Data;
            m_pData_4Data = NULL;
        }
    }

    bool CPChromeCacheParser::ParseFile( PSIHANDLE pFile)
    {
        if ( pFile == NULL )
        {
            return false;
        }

        m_pIndexFile = pFile;
        m_TimezoneRegion = siutil::MacCommonHelper::GetTimezoneRegion();
        m_pFloder = CFileSystem::GetItemParent(m_pIndexFile);

        std::vector<EntryStroreLoc> vEntryStores;// 用于存储从 index 文件中解析出的 entry 的位置
        uint32 version = 0;        // 用于保存当前解析chrome的版本号

        bool bSuc = ParseChromeCacheIndex( vEntryStores, &version);

        if ( !bSuc && vEntryStores.empty() ) // 解析index文件成功
        {
            return false;
        }

        size_t entrySize = vEntryStores.size();

        for ( size_t i = 0; i < entrySize; ++i )
        {
            ParseChromeCacheEntryStore( vEntryStores[i]);
        }
        return true;
    }

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
    bool CPChromeCacheParser::ParseChromeCacheIndex( std::vector<EntryStroreLoc>& vEntryStores, uint32* version)
    {
        uint64 uReadIndexFileLen = ReadIndexFile();
        if ( uReadIndexFileLen <= sizeof(IndexHeader) || uReadIndexFileLen > 20*1024*1024 )
        {
            return false;
        }

        IndexHeader header = *(IndexHeader*) m_pIndexData;
        *version = header.version;
        if ( header.version != kCurrentVersion20 && header.version!= kCurrentVersion21 ) // 只解析此两种版本的cache记录
        {
            return false;
        }
        if ( header.num_entries <= 0 )
        {
            return false;
        }
        uint64 addrNum =( uReadIndexFileLen - sizeof(IndexHeader) ) / sizeof(Addr);
        byte* pData = m_pIndexData + sizeof(IndexHeader);
        while ( addrNum )
        {

            Addr addr = *(Addr*)pData;
            if (addr.is_initialized())
            {
                EntryStroreLoc entryLoc;
                entryLoc.mFileNumber = addr.FileNumber();
                entryLoc.mBlockSize = addr.BlockSize();
                entryLoc.mStartPos = sizeof(BlockFileHeader) + addr.start_block() * addr.BlockSize();
                entryLoc.mSize = addr.num_blocks() * addr.BlockSize();
                vEntryStores.push_back( entryLoc );

            }
            pData += sizeof(CacheAddr);
            --addrNum;
        }
        size_t num = vEntryStores.size();
        return true;
    }


    uint64 CPChromeCacheParser::ReadIndexFile()
    {
       // uint64 uFileLength = CFileSystem::GetItemSize(m_pIndexFile);
         uint64 uFileLength = 2323233;
        uint64 uRealFileLength = uFileLength;

        if (  uFileLength > kMaxIndexFileLenth )
        {
            uRealFileLength = kMaxIndexFileLenth ;
        }
        if ( uRealFileLength > sizeof(IndexHeader) )
        {
            m_pIndexData = new byte[(size_t)uRealFileLength];
            uint64 uReadLength = CFileSystem::ReadFile(m_pIndexFile, 0, uRealFileLength, m_pIndexData);
            if(uReadLength > uFileLength)
            {
                uReadLength = uFileLength;
            }
            m_indexFileLength = uReadLength;
            return m_indexFileLength;
        }
        else
            return 0;
    }

    /*{=====================================================*//**
    @brief        解析entryStore
    @param        [输入]：    vEntryStores：    待解析出的所有entryStore
    @param        [输入]：    pProgressCtrl：    进度条
    @return        void
    @author        王虎
    @date        2011.10.10
    @remark
    *///}========================================================
    void CPChromeCacheParser::ParseChromeCacheEntryStore( const EntryStroreLoc& entryStore)
    {


        if ( m_pFloder == NULL )
        {
            return ;
        }
        // entry 只存在于 data_1 或 data_4 文件中
        if ( entryStore.mFileNumber == DATA_1 )
        {
            ReadData_1File();
        }else if ( entryStore.mFileNumber == DATA_4 )
        {
            ReadData_4File();
        }else
        {
            return ;
        }

        byte* pBegPos = NULL;
        uint64 nBufSize = 0;
        if ( entryStore.mFileNumber == DATA_1 && NULL != m_pData_1Data )
        {
            pBegPos = m_pData_1Data + entryStore.mStartPos;
            nBufSize = m_data_1FileLength;

        }
        else if ( entryStore.mFileNumber == DATA_4 && NULL != m_pData_4Data )
        {
            pBegPos = m_pData_4Data + entryStore.mStartPos;
            nBufSize = m_data_4FileLength;
        }
        if ( entryStore.mStartPos > nBufSize - sizeof(EntryStore) )
        {
            return;
        }
        if ( NULL == pBegPos)
        {
            return;
        }
        ParseChromeCacheEntry( pBegPos, nBufSize - entryStore.mStartPos );
    }



    /*{=====================================================*//**
    @brief        解析单条 entry
    @param        [输入]：    pData：    指向待解析的 entry 起始位置
    @param        [输入]：    nBufSize：    有效待解析buf的长度
    @return        void
    @author        王虎
    @date        2011.10.10
    @remark
    *///}========================================================
    void  CPChromeCacheParser::ParseChromeCacheEntry( byte* pData, uint64 nBufSize )
    {
        if ( NULL == pData || nBufSize < sizeof(EntryStore) )
        {
            return;
        }
        EntryStore entry = *(EntryStore*)pData;

		BrowserCache oneCache;
        if ( entry.rankings_node != 0x00 )
        {
            ParseRankingsNode( entry, oneCache );
        }

        if ( entry.long_key != 0x00 )
        {
            ParseEntryLongKey( entry, oneCache );
        }
        else if ( entry.key_len > 256 - 24 * 4 ) // (256 - 24 * 4) 是每一个entry单元所能存储key的最大字节数
		{
            ParseEntryKey( entry, pData, oneCache, nBufSize );
        }
        else
        {
            std::string tempA = entry.key;
            std::wstring tempW = CPCodeChanger::UTF8ToUnicode(tempA.c_str()); //CPCodeChanger::MbcsToUnicode( tempA.c_str(), tempA.size() );
            oneCache.url_ = tempW;

           /* size_t nameStart = tempW.rfind(_T('/'));
            std::wstring strEntryName;
            if ( nameStart != std::wstring::npos )
            {
                strEntryName = tempW.substr( nameStart + 1 );
                pWebRecord->SetEntryName( strEntryName );
            }*/
        }

		Caches_.push_back(oneCache);

        if ( entry.next != 0x00 )
        {
            ParseEntryNext( entry );
        }

    }


    /*{=====================================================*//**
    @brief        解析entry的key
    @param        [输入]：    entry：        待解析的 entry
    @param        [输入]：    bPegPos：    当前解析 entry 的起始位置
    @param        [输入]：    pWebRecord：    解析后的record
    @param        [输入]：    nBufSize：    有效待解析buf的长度
    @return        void
    @author        王虎
    @date        2011.10.10
    @remark
    *///}========================================================
    void CPChromeCacheParser::ParseEntryKey( const EntryStore& entry, byte* bPegPos, BrowserCache& oneCache, uint64 nBufSize )
    {
        if ( nBufSize < (uint64)(0x60 + entry.key_len) ) // 不够长
        {
            return;
        }
        std::string tempA( (char*)( bPegPos+0x60 ), entry.key_len + 1 );
        std::wstring tempW = CPCodeChanger::UTF8ToUnicode(tempA.c_str()); //CPCodeChanger::MbcsToUnicode( tempA.c_str(), tempA.size() );
        oneCache.url_ = tempW;

        /*size_t nameStart = tempW.rfind(L'/');
        std::wstring strEntryName;
        if ( nameStart != std::wstring::npos )
        {
            strEntryName = tempW.substr( nameStart + 1 );
            pWebRecord->SetEntryName( strEntryName );
        }*/

    }

    /*{=====================================================*//**
    @brief        解析entry的httpheader
    @param        [输入]：    entry：        待解析的 entry
    @param        [输入]：    pWebRecord：    解析后添加到的record
    @return        void
    @author        王虎
    @date        2011.10.10
    @remark
    *///}========================================================
//    void CPChromeCacheParser::PsrseEntryHttpHeader( const EntryStore& entry, CPWebRecord* pWebRecord )
//    {
//        Addr httpAddr( entry.data_addr[0] );
//        if ( !httpAddr.is_initialized() || !httpAddr.is_block_file() )
//        {
//            return;
//        }

//        uint32 uStartPos = sizeof(BlockFileHeader) + httpAddr.BlockSize() * httpAddr.start_block();

//        size_t nLength = 0;
//        byte* pData = GetDataPointerByFileNum( httpAddr.FileNumber(), &nLength );
//        if ( NULL == pData || uStartPos + entry.data_size[0] >= nLength)
//        {
//            return ;
//        }


//        HttpHeader* pHeader = (HttpHeader*)( pData + uStartPos );
//        if (pHeader->length >= entry.data_size[0] || sizeof(HttpHeader) > entry.data_size[0])
//        {
//            return ;
//        }

//        std::wstring strInfo;
//        byte* buf = pData + uStartPos +sizeof(HttpHeader);
//        size_t nBegin = 0;
//        size_t i = 0;
//        for (i=0; i<pHeader->length; i++)
//        {
//            if (buf[i] == 0)
//            {
//                if (nBegin != i)
//                {
//                    if (!strInfo.empty())
//                    {
//                        strInfo += _T(" ");
//                    }

//                    strInfo += CPCodeChanger::MbcsToUnicode((char*)(buf+nBegin), i-nBegin);
//                }

//                nBegin = i+1;
//            }
//        }

//        if (i == pHeader->length)
//        {
//            if (!strInfo.empty())
//            {
//                strInfo += _T(" ");
//            }

//            strInfo += CPCodeChanger::MbcsToUnicode((char*)(buf+nBegin), i-nBegin);
//        }

//        pWebRecord->SetHeaderInfo( strInfo );
//        pWebRecord->SetRemark( strInfo );
//    }

    /*{=====================================================*//**
    @brief        解析entry的真实缓存的数据内容
    @param        [输入]：    entry：        待解析的 entry
    @param        [输入]：    pWebRecord：    解析后添加到的record
    @return        void
    @author        王虎
    @date        2011.10.10
    @remark        只有少数的entry的key是该种情况
    *///}========================================================
//    void CPChromeCacheParser::ParseEntryActualData( const EntryStore& entry, CPWebRecord* pWebRecord )
//    {
//        Addr address( entry.data_addr[1] );
//        if ( !address.is_initialized())
//        {
//            return;
//        }
//        if ( address.is_block_file() )
//        {
//            uint32 fileNumber = address.FileNumber();
//            uint32 nBlockSize = 0;
//            nBlockSize = address.BlockSize();

//            if ( fileNumber == 0x01 )
//            {
//                if ( !m_pData_1File && m_pFloder!= NULL )
//                {
//                    m_pData_1File = (PSIHANDLE)m_pFloder->FindChild(_T("data_1"));
//                }
//                pWebRecord->SetMapFile( m_pData_1File );
//                pWebRecord->SetSrcFile( m_pData_1File );
//            }
//            else if ( fileNumber == 0x02 )
//            {
//                if ( !m_pData_2File && m_pFloder!= NULL )
//                {
//                    m_pData_2File = (PSIHANDLE)m_pFloder->FindChild(_T("data_2"));
//                }
//                pWebRecord->SetMapFile( m_pData_2File);
//                pWebRecord->SetSrcFile( m_pData_2File);
//            }
//            else if ( fileNumber = 0x03 )
//            {
//                if ( !m_pData_3File && m_pFloder != NULL )
//                {
//                    m_pData_3File = (PSIHANDLE)m_pFloder->FindChild( _T("data_3") );
//                }
//                pWebRecord->SetMapFile( m_pData_3File );
//                pWebRecord->SetSrcFile( m_pData_3File );
//            }
//            else if ( fileNumber == 0x04 )
//            {
//                if ( !m_pData_4File && m_pFloder != NULL )
//                {
//                    m_pData_4File = (PSIHANDLE)m_pFloder->FindChild(_T("data_4") );
//                }
//                pWebRecord->SetMapFile( m_pData_4File );
//                pWebRecord->SetSrcFile( m_pData_4File );
//            }

//            if ( !nBlockSize )
//            {
//                return;
//            }

//            uint32 nStart = sizeof(BlockFileHeader) + address.start_block()*nBlockSize;
//            uint32 nSize = entry.data_size[1]; // actual data 存在于下标为 1 的数据流中

//            pWebRecord->SetMapFileStart( nStart );
//            pWebRecord->SetMapFileSize( nSize );

//            pWebRecord->SetPosInSrc( nStart );
//            pWebRecord->SetSizeInSrc( nSize );

//        }
//        else
//        {
//            std::wstring strFileName = address.GetSeparateFileName();
//            PSIHANDLE pSepFile = (PSIHANDLE)m_pFloder->FindChild( strFileName );
//            if ( pSepFile )
//            {
//                pWebRecord->SetMapFile( pSepFile );
//                pWebRecord->SetMapFileStart( 0 );
//                pWebRecord->SetMapFileSize( pSepFile->GetItemFileLength() );

//                pWebRecord->SetSrcFile( pSepFile );
//                pWebRecord->SetPosInSrc( 0 );
//                pWebRecord->SetSizeInSrc( pSepFile->GetItemFileLength() );
//            }
//        }
//    }


    /*{=====================================================*//**
    @brief        解析entry的long key
    @param        [输入]：    entry：        待解析的 entry
    @param        [输入]：    pWebRecord：    解析后的record
    @return        void
    @author        王虎
    @date        2011.10.10
    @remark        只有少数的entry的key是该种情况
    *///}========================================================
    void CPChromeCacheParser::ParseEntryLongKey( const EntryStore& entry, BrowserCache& oneCache )
    {

        Addr longKeyAddr( entry.long_key );
        bool bDataSep = false;
        if ( !longKeyAddr.is_initialized())
        {
            return;
        }
        byte* pData = NULL;
        uint64 nLength = 0;
        uint32 fileNum = longKeyAddr.FileNumber();
        uint64 nBlockSize = 0;
        uint64 startPos = 0;
        uint64 size = 0;

        if ( fileNum >= DATA_0 && fileNum <= DATA_4 )
        {
            nBlockSize = longKeyAddr.BlockSize();
        }
        if ( fileNum== DATA_0 )
        {
            if ( NULL == m_pData_1Data  )
            {
                ReadData_1File();
            }
        }
        else if ( fileNum == DATA_2 )
        {
            if (NULL == m_pData_2Data)
            {
                ReadData_2File();
            }
        }
        else if ( fileNum== DATA_3 )
        {
            if ( NULL == m_pData_3Data )
            {
                ReadData_3File();
            }
        }
        else if ( fileNum == DATA_4 )
        {
            if ( NULL == m_pData_4Data )
            {
                ReadData_4File();
            }
        }
        else
        {
            std::wstring longKeyFileName = longKeyAddr.GetSeparateFileName();
            PSIHANDLE pKeyFile = NULL;
            size = ReadDataFileByFileName( longKeyFileName, pKeyFile, pData, nLength);
            startPos = 0;
            if (NULL == pData)
            {
                bDataSep = true;
            }
        }
        if ( !bDataSep )
        {
            pData = GetDataPointerByFileNum( fileNum, (size_t*)&nLength);
            startPos = sizeof(BlockFileHeader) + longKeyAddr.start_block()*nBlockSize;
            size = entry.key_len;
        }
        //添加了指针位置的判断，简化了逻辑处理，by 丁显鹏,2014.1.2
        if ( pData == NULL || startPos + size > nLength)
        {
            return;
        }

        std::string longKeyA( (char*)pData+startPos, (size_t)size );
        std::wstring longKeyW = CPCodeChanger::UTF8ToUnicode(longKeyA.c_str()); //CPCodeChanger::MbcsToUnicode( longKeyA.c_str(), longKeyA.size() );
        oneCache.url_ = longKeyW;

//        size_t nameStart = longKeyW.rfind(L'/');
//        std::wstring strEntryName;
//        if ( nameStart != std::wstring::npos )
//        {
//            strEntryName = longKeyW.substr( nameStart + 1 );
//            pWebRecord->SetEntryName( strEntryName );
//        }

        if (bDataSep) // 独立存储的 Key 解析后清空内存
        {
            delete[] pData;
        }

    }

    /*{=====================================================*//**
    @brief        解析entry的rankingsNode
    @param        [输入]：    entry：        待解析的 entry
    @param        [输入]：    pWebRecord：    解析后添加到的record
    @return        void
    @author        王虎
    @date        2011.10.10
    @remark        主要是为了获得该entry的othertime和accesstime
    *///}========================================================
    void CPChromeCacheParser::ParseRankingsNode( const EntryStore& entry,  BrowserCache& oneCache )
    {
        Addr rankingAddr(entry.rankings_node);
        uint32 fileNum = rankingAddr.FileNumber();
        if ( rankingAddr.is_initialized() && rankingAddr.FileNumber() == 0x00 )
        {
            ReadData_0File();
            uint32 startPos = sizeof(BlockFileHeader) + rankingAddr.start_block() * 36;

            if ( m_data_0FileLength < startPos + sizeof(RankingsNode) )
            {
                return;
            }
            RankingsNode rankNode = *(RankingsNode*)(m_pData_0Data + startPos);
            //ParseRankNodeNext( rankNode, pWebRecord );
            //ParseRankNodePrev( rankNode, pWebRecord );
            //ParseRankNodeContent( rankNode, pWebRecord );
            uint64 lastUsed = rankNode.last_used;
            lastUsed *= 10;  // 补足18位数字
           // CPTime lastUsedTime( CPTime::LocalTime2UTC(lastUsed));//Int64ToFileTime
          //  lastUsedTime = CPTime::UTC2LocalTime(lastUsedTime , m_TimezoneRegion);
          //  oneCache.visit_time_ = lastUsedTime; // 最后访问时间设为 lastused

            uint64 lastModifed = rankNode.last_modified;
            lastModifed *= 10; // 补足18位数字
            //CPTime lastModifedTime(CPTime::DecodeMailTime(lastModifed));//Int64ToFileTime

          //  lastModifedTime = CPTime::UTC2LocalTime( lastModifedTime, m_TimezoneRegion );//UTC2LocalTime
//            pWebRecord->SetOtherTime( lastModifedTime ); // 其它时间设为 lastused
        }

    }

    /*{=====================================================*//**
    @brief        解析单条 entry 的下一条 entry
    @param        [输入]：    entry：    待解析的 entry
    @return        void
    @author        王虎
    @date        2011.10.10
    @remark        并是不所有的entry都会有 next entry
    *///}========================================================
    void CPChromeCacheParser::ParseEntryNext( const EntryStore& entry )
    {

        Addr nextEntry( entry.next );

        uint32 fileNum = nextEntry.FileNumber();
        byte* pData = NULL;

        size_t nLength = 0;
        pData = GetDataPointerByFileNum( fileNum, &nLength );
        if ( pData == NULL )
        {
            return;
        }
        uint32 nBlockSize = nextEntry.BlockSize();

        uint32 startPos = nextEntry.start_block() * nBlockSize + sizeof(BlockFileHeader);
        uint32 size = nextEntry.num_blocks()*nBlockSize;
        if (startPos >= nLength)// 起始位置出错
        {
            return;
        }
        ParseChromeCacheEntry( pData+startPos, nLength-startPos );

    }

    bool CPChromeCacheParser::ReadData_0File()
    {
        if ( NULL == m_pData_0File && NULL == m_pData_0Data )
        {
            std::wstring fileName(L"data_0");
            uint64 uReadFileLength = ReadDataFileByFileName(fileName, m_pData_0File, m_pData_0Data, m_data_0FileLength);
            if ( 0 == uReadFileLength  || uReadFileLength > kMaxDataFileLength )
            {
                return false;
            }
            if ( uReadFileLength == CFileSystem::GetItemSize(m_pData_0File) || uReadFileLength == kMaxDataFileLength)
            {
                return true;
            }
        }
        return false;
    }

    bool CPChromeCacheParser::ReadData_1File()
    {
        if ( NULL == m_pData_1File && NULL == m_pData_1Data )
        {
            std::wstring fileName(L"data_1");
            uint64 uReadFileLength = ReadDataFileByFileName(fileName, m_pData_1File, m_pData_1Data, m_data_1FileLength );
            if ( !uReadFileLength  || uReadFileLength > kMaxDataFileLength )
            {
                return false;
            }
            if ( uReadFileLength == CFileSystem::GetItemSize(m_pData_1File) || uReadFileLength == kMaxDataFileLength)
            {
                return true;
            }
        }
        return false;
    }


    bool CPChromeCacheParser::ReadData_2File()
    {
        if ( NULL == m_pData_2File && NULL == m_pData_2Data )
        {
            std::wstring fileName(L"data_2");
            uint64 uReadFileLength = ReadDataFileByFileName(fileName, m_pData_2File, m_pData_2Data, m_data_2FileLength );
            if ( !uReadFileLength  || uReadFileLength > kMaxDataFileLength ) // 文件长度过大或为0
            {
                return false;
            }
            if ( uReadFileLength == CFileSystem::GetItemSize(m_pData_2File) || uReadFileLength == kMaxDataFileLength) // 读取与实际不相符
            {
                return true;
            }
        }
        return false;
    }

    bool CPChromeCacheParser::ReadData_3File()
    {
        if ( NULL == m_pData_3File && NULL == m_pData_3Data )
        {
            std::wstring fileName(L"data_3");
            uint64 uReadFileLength = ReadDataFileByFileName(fileName, m_pData_3File, m_pData_3Data, m_data_3FileLength );
            if ( 0 == uReadFileLength  || uReadFileLength > kMaxDataFileLength ) // 文件长度过大或为0
            {
                return false;
            }
            if ( uReadFileLength == CFileSystem::GetItemSize(m_pData_3File) || uReadFileLength == kMaxDataFileLength) // 读取与实际不相符
            {
                return true;
            }
        }
        return false;
    }

    bool CPChromeCacheParser::ReadData_4File()
    {
        if ( NULL == m_pData_4File && NULL == m_pData_4Data )
        {
            std::wstring fileName(L"data_4");
            uint64 uReadFileLength = ReadDataFileByFileName(fileName, m_pData_4File, m_pData_4Data, m_data_4FileLength );
            if ( 0 == uReadFileLength  || uReadFileLength > kMaxDataFileLength ) // 文件长度过大或为0
            {
                return false;
            }
            if ( uReadFileLength == CFileSystem::GetItemSize(m_pData_4File) || uReadFileLength == kMaxDataFileLength) // 读取与实际不相符
            {
                return true;
            }
        }
        return false;
    }


    uint64 CPChromeCacheParser::ReadDataFileByFileName( const std::wstring& strFileName, PSIHANDLE& pFile, byte*& pData, uint64& fileLength )
    {
        if ( NULL != pFile || NULL != pData )
        {
            return 0;
        }
		
        pFile = NULL;
		int subCount = CFileSystem::SubItemCount(m_pFloder);
		for (int i = 0; i < subCount; ++i)
		{
			PSIHANDLE subHandle = CFileSystem::GetSubItemHandle(m_pFloder, i);
			if (!CFileSystem::IsItemDeleted(subHandle) && strFileName == CFileSystem::GetItemName(subHandle))
			{
				pFile = subHandle;
				break;
			}
		}

        if ( NULL == pFile )
        {
            return 0;
        }
        uint64 uFileLength = CFileSystem::GetItemSize(pFile);
        if ( uFileLength < sizeof(BlockFileHeader) )
        {
            return 0;
        }
        if ( uFileLength <= kMaxDataFileLength )
        {
            pData = new byte[(unsigned int)uFileLength];
            CFileSystem::ReadFile(pFile, 0, uFileLength, pData);
			fileLength = uFileLength;
            return uFileLength;
        }
        return 0;
    }

    byte* CPChromeCacheParser::GetDataPointerByFileNum( const uint32 uFileNum, size_t* nLength )
    {
        PSIHANDLE pFile = GetFilePointerByFileNum(uFileNum);
        if (NULL == pFile || CFileSystem::GetItemSize(pFile) > 100*1024*1024)
        {
            return NULL;
        }

        *nLength = (size_t)CFileSystem::GetItemSize(pFile);

        byte* pData = NULL;
        switch( uFileNum )
        {
        case DATA_0:
            pData = m_pData_0Data; break;
        case DATA_1:
            pData = m_pData_1Data; break;
        case DATA_2:
            pData = m_pData_2Data; break;
        case DATA_3:
            pData = m_pData_3Data; break;
        case DATA_4:
            pData = m_pData_4Data; break;
        default:
            break;
        }
        return pData;
    }

    PSIHANDLE CPChromeCacheParser::GetFilePointerByFileNum( const uint32 uFileNum )
    {
        PSIHANDLE pFile = NULL;
        switch( uFileNum )
        {
        case DATA_0:
            pFile = m_pData_0File; break;
        case DATA_1:
            pFile = m_pData_1File; break;
        case DATA_2:
            pFile = m_pData_2File; break;
        case DATA_3:
            pFile = m_pData_3File; break;
        case DATA_4:
            pFile = m_pData_4File; break;
        default:
            break;
        }
        return pFile;
    }

	void CPChromeCacheParser::GetCaches( BrowserCaches& caches )
	{
		caches = Caches_;
	}

};
