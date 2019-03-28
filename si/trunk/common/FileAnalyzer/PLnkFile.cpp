#include "stdafx.h"
#include "FileAnalyzer/PLnkFile.h"

using namespace filesystem;

namespace pfile
{

#define FLAG_BIT(x) (1 << (x))
#pragma pack(1)

	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned __int64 uint64;

	const uint64 LINK_CLSID_LO = 0x21401;
	const uint64 LINK_CLSID_HI = 0x46000000000000C0;

	struct ShellLinkHeader
	{
		uint32      HeaderSize;     //Must be 0x4C
		uint64      LinkCLSID_lo;
		uint64      LinkCLSID_hi; //A class identifier (CLSID). This value MUST be 00021401-0000-0000-C000-000000000046.
		uint32      LinkFlags;
		uint32      FileAttributes;
		FILETIME    CreationTime;
		FILETIME    AccessTime;
		FILETIME    WriteTime;
		uint32      FileSize;
		uint32      IconIndex;
		uint32      ShowCommand;
		uint16      HotKey;
		uint16      Reserved1;
		uint32      Reserved2;
		uint32      Reserved3;
	};
	enum LinkFlags
	{
		HasLinkTargetIDList = FLAG_BIT(0),
		HasLinkInfo = FLAG_BIT(1),
		HasName = FLAG_BIT(2),
		HasRelativePath = FLAG_BIT(3),
		HasWorkingDir = FLAG_BIT(4),
		HasArguments = FLAG_BIT(5),
		//由于不需要，接下来的一些标志位未列出
	};

	//目标文件的属性，与winnt.h中定义的枚举类型相同
	enum TargetFileAttributesFlags
	{
		TARGET_FILE_ATTRIBUTE_READONLY = FLAG_BIT(0),
		TARGET_FILE_ATTRIBUTE_HIDDEN = FLAG_BIT(1),
		TARGET_FILE_ATTRIBUTE_SYSTEM = FLAG_BIT(2),
		RESERVED1 = FLAG_BIT(3), //Reserved,must be 0
		TARGET_FILE_ATTRIBUTE_DIRECTORY = FLAG_BIT(4),
	};

	enum LinkInfoFlags
	{
		HasVolumeIDAndLocalBasePath = FLAG_BIT(0),
		HasCommonNetRelLinkAndPathSuffix = FLAG_BIT(1)
	};
	struct LinkInfoHeader
	{
		uint32      LinkInfoSize;
		uint32      LinkInfoHeaderSize;
		uint32      LinkInfoFlags;
		uint32      VolumeIDOffset;
		uint32      LocalBasePathOffset;
		uint32      CommonNetRelLinkOffset;
		uint32      CommonPathSuffixOffset;
	};

	struct VolumeIDHeader
	{
		uint32      VolumeIDSize;
		uint32      DriveType;
		uint32      DriveSerialNumber;
		uint32      VolumeLabelOffset;
	};

	enum CommonNetRelLinkFlags
	{
		ValidDevice = FLAG_BIT(0),
		ValidNetType = FLAG_BIT(1)
	};
	struct CommonNetRelLinkFlagsHeader
	{
		uint32      Size;
		uint32      Flags;
		uint32      NetNameOffset;
		uint32      DeviceNameOffset;
		uint32      NetworkProviderType;
	};

#pragma pack()


	CPLnkFile::CPLnkFile(PSIHANDLE handle)
		: _parsed(false), m_pFile(handle)
	{

	}

	std::wstring CPLnkFile::targetPath()
	{
		if (!_parsed) {
			ParseFile();
			_parsed = true;
		}
		return m_TargetPath;
	}

	bool CPLnkFile::ParseFile()
	{
		if (m_pFile == NULL) {
			return false;
		}
		size_t fileSize = (size_t)CFileSystem::GetItemSize(m_pFile);
		//1MB,但文件长度大于1MB时，不解析
		if (fileSize > 1024 * 1024) {
			return false;
		}
		std::vector<char> vbuf(fileSize);
		char* buf = vbuf.data();

		if (fileSize != CFileSystem::ReadFile(m_pFile, 0, fileSize, (uchar*)buf)) {
			return false;
		}
		ShellLinkHeader* LinkHeader = (ShellLinkHeader*)buf;
		if (LinkHeader->HeaderSize != 0x4c || LinkHeader->LinkCLSID_lo != LINK_CLSID_LO ||
			LinkHeader->LinkCLSID_hi != LINK_CLSID_HI)
		{
			return false;
		}
		// 当文件中包括LinkInfo结构时，才解析其中的目标文件路径，暂不考虑相对路径节和工作目录节
		if ((LinkHeader->LinkFlags & HasLinkInfo) == 0)
		{
			return false;
		}
		unsigned short IDListSize = *(unsigned short*)(buf + LinkHeader->HeaderSize);
		uint32 LinkInfoOffset = LinkHeader->HeaderSize;
		if (LinkHeader->LinkFlags & HasLinkTargetIDList)
		{
			LinkInfoOffset += IDListSize + sizeof(IDListSize);
		}
		if (LinkInfoOffset > fileSize)
		{
			return false;
		}
		LinkInfoHeader* pLinkInfoheader = (LinkInfoHeader*)(buf + LinkInfoOffset);
		if (pLinkInfoheader->LinkInfoSize > fileSize ||
			(pLinkInfoheader->LinkInfoHeaderSize != 0x1C && pLinkInfoheader->LinkInfoHeaderSize != 0x24))
		{
			return false;
		}

		//目标文件是本地文件，只解析BasePath即可
		if (pLinkInfoheader->LinkInfoFlags & HasVolumeIDAndLocalBasePath)
		{
			if (pLinkInfoheader->LocalBasePathOffset != 0 &&
				pLinkInfoheader->VolumeIDOffset != 0)
			{
				uint32 BasePathOffset = LinkInfoOffset + pLinkInfoheader->LocalBasePathOffset;
				if (BasePathOffset > fileSize)
				{
					return false;
				}
				m_TargetPath = GB2W(buf + BasePathOffset);
				return true;
			}
		}
		//目标文件是网路文件，需要解析CommonNetRelLink结构
		else if (pLinkInfoheader->LinkInfoFlags & HasCommonNetRelLinkAndPathSuffix)
		{
			if (pLinkInfoheader->CommonNetRelLinkOffset != 0 &&
				pLinkInfoheader->CommonPathSuffixOffset != 0)
			{
				//1、获得网络路径
				uint32 CommonNetRelLinkOffset = LinkInfoOffset + pLinkInfoheader->CommonNetRelLinkOffset;
				if (CommonNetRelLinkOffset > fileSize)
				{
					return false;
				}
				CommonNetRelLinkFlagsHeader* pNetRelLinkHeader = (CommonNetRelLinkFlagsHeader*)(buf + CommonNetRelLinkOffset);
				uint32 NetPathOffset = CommonNetRelLinkOffset + pNetRelLinkHeader->NetNameOffset;
				if (NetPathOffset > fileSize)
				{
					return false;
				}
				std::wstring NetName = GB2W(buf + NetPathOffset);

				//2、获得文件名
				uint32 FileNameOffset = LinkInfoOffset + pLinkInfoheader->CommonPathSuffixOffset;
				if (FileNameOffset > fileSize)
				{
					return false;
				}
				std::wstring FileName = GB2W(buf + FileNameOffset);
				m_TargetPath = NetName + L"\\" + FileName;
				return true;
			}
		}
		return false;
	}
}
