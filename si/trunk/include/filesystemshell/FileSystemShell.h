#ifndef SI_FS_FILESYSTEM_HELPER_H
#define SI_FS_FILESYSTEM_HELPER_H
#include "./globals.h"
#include "./PInterfaceStruct.h"

namespace filesystem
{
	//SI文件系统对外的结构函数簇
	class  FSS_API CFileSystem
	{
	public:
		/*{=====================================================*//**
		@brief		文件系统初始化
		@return		bool：		成功返回true
		@author		丁显鹏
		@date		2014.10.15
		@remark		主要加载DLL,并导出相关函数
		*///}========================================================
        static bool Initialize(const std::wstring& dllDir);
		/*{=====================================================*//**
		@brief		得到本地设备（磁盘）的数量
		@param		无
		@return		int：					本地设备的数目
		@author		丁显鹏
		@date		2014.10.15
		@remark		如果有错误，则返回 -1
		*///}========================================================
		static int LocalDeviceCount();
		/*{=====================================================*//**
		@brief		得到设备的信息
		@param		[输入]：	index：	设备的索引
		@param		[输出]：	pItem：	设备信息
		@return		PSIHANDLE：       设备的句柄
		@author		丁显鹏
		@date		2008.10.15
		@remark
		*///}========================================================
		static PSIHANDLE GetDeviceInfo(unsigned int index, SPDevice* pDevice);
		/*{=====================================================*//**
		@brief		 通过句柄得到设备的信息
		@param		[输入]：	hDevice：设备的句柄
		@param		[输出]：	pItem：	设备信息
		@return		bool : 成功返回ture
		@author		丁显鹏
		@date		2008.10.27
		@remark
		*///}========================================================
		static bool GetDeviceInfo(const PSIHANDLE hDevice, SPDevice* pItem);
		/*{=====================================================*//**
		@brief		加载设备并加载文件系统
		@param		[输入]：	hDevice：		加载设备的句柄
		@return		bool：		正常返回true，否则返回false
		@author		丁显鹏
		@date		2014.10.15
		@remark		请注意：这个步骤可能视文件的多少，要花费比较长的时间
		*///}========================================================
		static bool LoadDevice(PSIHANDLE hDevice);
		/*{=====================================================*//**
		@brief		释放指定设备
		@param		[输入]：    hItem：     设备的句柄
		@return		void
		@author		丁显鹏
		@date		2014.10.15
		@remark		释放设备，删除设备下的卷和文件
		*///}========================================================
		static bool ReleaseDevice(PSIHANDLE hDevice);
		/*{=====================================================*//**
		@brief		释放所有设备
		@param		[输入]：    hItem：     设备的句柄
		@return		void
		@author		丁显鹏
		@date		2014.10.15
		@remark		释放设备，删除设备下的卷和文件
		*///}========================================================
		static void ReleaseAllDevice();

		/*{=====================================================*//**
		@brief		更新文件系统的时区
		@param		[输入]：    hDevice|hVolume：     设备|卷的句柄
		@param		[输入]：    bias：时区偏差（分钟数），如东八区，bias=-480(-8*60);
		@                            西八区 ：bias = 480（8*60）
		@return		bool
		@author		丁显鹏
		@date		2015.10.22
		@remark		更新文件系统的时区,windows系统中已经调整到本地时区
		@           linux系统中已经调整到默认的东八区（bias = -480）
		*///}========================================================
		static bool UpdateDeviceTimeZone(PSIHANDLE hDevice, long bias);
		static bool UpdateVolumeTimeZone(PSIHANDLE hVolume, long bias);

		/*{=====================================================*//**
		@brief		返回某个item的类型
		@param		[输入]：	hItem：      当前句柄
		@return     ITEM_TYPE：          类型
		@author		丁显鹏
		@date		2014.10.23
		@remark		返回hItem的类型，无效的hitem返回ITEM_TYPE_NONE
		*///}========================================================
		static ITEM_TYPE GetItemType(const PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief      判断设备是否为USB设备
		@param		[输入]：    hItem：     设备的句柄
		@return		bool
		@author		丁显鹏
		@date		2015.10.14
		@remark		GetLocalDevice返回的设备中包含USB设备
		*///}========================================================
		static bool IsUsbDevice(PSIHANDLE hDevice);

		/*{=====================================================*//**
		@brief		得到单个设备下卷的数量
		@param		[输入]：	hDevice：   当前设备的句柄
		@return		int：			    当前设备中卷的数目
		@author		丁显鹏
		@date		2014.10.15
		@remark		如果有错误，则返回 -1
		*///}========================================================
		static int VolumeCount(const PSIHANDLE hDevice);


		/*{=====================================================*//**
		@brief		得到逻辑卷的数量
		@return		int：			    当前设备中卷的数目
		@author		丁显鹏
		@date		2014.10.15
		@remark		如果有错误，则返回 -1
		*///}========================================================
		static int LogicalVolumeCount();

		/*{=====================================================*//**
		@brief		得到卷的信息
		@param		[输入]：	hDevice：		当前设备的句柄
		@param		[输入]：	index：		    卷的索引
		@param		[输出]：	pItem：		    卷的信息
		@return		PSIHANDLE：				卷的句柄
		@author		丁显鹏
		@date		2014.10.15
		@remark
		*///}========================================================

		static PSIHANDLE GetVolumeInfo(const PSIHANDLE hDevice, int index, SPVolume* pVolume);

		/*{=====================================================*//**
		@brief		得到卷的信息
		@param		[输入]：	hVolume：		当前卷的句柄
		@param		[输出]：	pItem：		    卷的信息
		@return		bool：				    成功返回true
		@author		丁显鹏
		@date		2014.10.27
		@remark
		*///}========================================================
		static bool GetVolumeInfo(const PSIHANDLE hVolume, SPVolume* pItem);


		/*{=====================================================*//**
		@brief		得到逻辑卷的信息
		@param		[输入]：	index：		卷的索引
		@param		[输出]：	pItem：		卷的结构
		@return		PHANDLE：		    卷的句柄
		@author		丁显鹏
		@date		2015.4.3
		@remark     某些通过mount工具挂载到本地的卷，并非真正的物理设备，如truecrypt，
		@           FTK Imager等挂载的镜像，这些卷没有类似磁盘的父设备，尽管挂载的源是
		@           一个镜像文件或加密的分区，在这里不需要通过父设备的handle来获取
		*///}========================================================
		static PSIHANDLE GetLogicalVolumeInfo(unsigned int index, SPVolume* pItem);

		/*{=====================================================*//**
		@brief		得到文件项的数量
		@param		[输入]：	hParent：		当前卷/目录的句柄
		@return		int：					当前卷/目录中的子文件项的数目
		@author		丁显鹏
		@date		2014.10.18
		@remark		1、如果 hParent 表示卷，则返回其根目录下的文件项数目
		@n			2、这是直接子文件项，不递归包含子目录中的文件项
		@n			2、如果有错误，则返回 -1
		*///}========================================================
		static int SubItemCount(const PSIHANDLE hParent);

		/*{=====================================================*//**
		@brief		得到卷/目录下的文件信息
		@param		[输入]：	hParent：	当前卷/目录的句柄
		@param		[输入]：	index：		文件项的索引
		@param		[输出]：	pFileItem： 文件项的结构
		@return		PSIHANDLE：		    文件项的句柄
		@author		丁显鹏
		@date		2014.10.15
		@remark
		*///}========================================================
		static PSIHANDLE GetFileItem(const PSIHANDLE hParent, unsigned int index, SPFileItem* pFileItem);

		//目前离线下通过实际路径找不到handle
#ifdef OS_PWINDOWS
		/*{=====================================================*//**
		@brief		通过路径得到文件项的句柄
		@param		[输入]：path：		文件的路径
		@return		PSIHANDLE：		文件项的句柄
		@author		张文攀
		@date		2015.4.1
		@remark
		*///}========================================================
		static PSIHANDLE GetItemHandleByPath(const std::wstring& path);
#endif

		/*{=====================================================*//*
		@brief		通过相对路径得到所有卷下的文件项的句柄, 可用文件名通配符“ * ”来表示路径中某些字符串
		@param		[输入]：path：		卷下的文件项的相对路径
		@param		[输入]: CaseSensitive         是否区分大小写
		@param		[输出]：handles：		获取的匹配的文件项(只对handles进行添加操作，不进行清空等操作)
		@return		bool:                是否有找到文件
		@author		张文攀
		@date		2015.7.31
		@remark
		*///}========================================================
		static bool GetItemHandlesByRawPath(const std::wstring& path, std::vector<PSIHANDLE>& handles, bool CaseSensitive = false);

		/*{=====================================================*//**
		@brief		通过相对路径得到指定根目录下的文件项的句柄
		@param		[输入]: pRootHandle      根目录句柄
		@param		[输入]：path：		卷下的文件项的相对路径
		@param		[输入]: CaseSensitive 是否区分大小写
		@return		PSIHANDLE:         匹配的文件项
		@author		张文攀
		@date		2015.8.3
		@remark
		*///}========================================================
		static PSIHANDLE GetItemHandleByRawPath(PSIHANDLE pRootHandle, const std::wstring& path, bool CaseSensitive = false);

		/*{=====================================================*//*
		@brief		通过相对路径得到指定根目录下的文件项的句柄, 可用文件名通配符“ * ”来表示路径中某些字符串
		@param		[输入]: pRootHandle      根目录句柄
		@param		[输入]：path：		根目录下的文件项的相对路径
		@param		[输出]：handles：		获取的匹配的文件项(只对handles进行添加操作，不进行清空等操作)
		@param		[输入]: CaseSensitive 是否区分大小写
		@return		PSIHANDLE:         匹配的文件项
		@author		张文攀
		@date		2015.8.3
		@remark
		*///}========================================================
		static bool GetItemHandlesByRawPath(PSIHANDLE pRootHandle, const std::wstring& path,
			std::vector<PSIHANDLE>& handles, bool CaseSensitive = false);

		/*{=====================================================*//**
		@brief		获取所有卷的句柄, 已过滤safeimager卷
		@param		[输出] pVolumes:		获取的卷句柄
		@author		张文攀
		@date		2015.8.3
		@remark
		*///}========================================================
		static void GetAllVolumes(std::vector<PSIHANDLE>& pVolumes);

		/*{=====================================================*//**
		@brief		判断两个handle是否在同一个卷下
		@param		[输入] pItem1:		项句柄
		@param		[输入] pItem2:		项句柄
		@author		张文攀
		@date		2015.8.25
		@remark
		*///}========================================================
		static bool InSameVolume(PSIHANDLE pItem1, PSIHANDLE pItem2);

		/*{=====================================================*//**
		@brief		判断pItem是否pRootItem的子项(递归)
		@param		[输入]：	pRootItem   根句柄
		@param		[输入]：	pItem   子句柄
		*///}========================================================
		static bool IsSubHandle(const PSIHANDLE pRootItem, PSIHANDLE pItem);

		/*{=====================================================*//**
		@brief		通过文件的句柄得到文件项的信息
		@param		[输入]：	hItem：		当文件/目录的句柄
		@param		[输出]：	pItem：		文件项的结构
		@return		bool:               正常返回true
		@author		丁显鹏
		@date		2014.10.20
		@remark		仅取文件或目录的信息，如果不是文件或目录的句柄，返回false
		*///}========================================================
		static bool GetFileItemInfo(const PSIHANDLE hItem, SPFileItem* pItem);


		/*{=====================================================*//**
		@brief		读取该文件内容
		@param		[输入]：	hFile：		当前文件的句柄
		@param		[输入]：	begin：		文件内的偏移
		@param		[输入]：	offset： 	要读取的长度
		@param		[输出]：	buf：		缓存
		@return		int：				实际读取的长度
		@author		丁显鹏
		@date		2014.10.15
		@remark
		*///}========================================================
		static unsigned __int64 ReadFile(
			const PSIHANDLE hFile,
			unsigned __int64 offset,
			unsigned __int64 length,
			unsigned char* buf);

		/*{=====================================================*//**
		@brief		返回某个item的打勾状态
		@param		[输入]：	hItem：		当前文件的句柄
		@return		ITEM_CHECK_STATE：	打勾状态
		@author		丁显鹏
		@date		2014.10.15
		@remark
		*///}========================================================
		static ITEM_CHECK_STATE ItemCheckState(const PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		返回某个Item的父节点
		@param		[输入]：	hItem：		 句柄
		@return		PSIHANDLE：            父节点Handle
		@author		丁显鹏
		@date		2014.10.15
		@remark
		*///}========================================================
		static PSIHANDLE GetItemParent(const PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		判断某个Item是否是文件
		@param		[输入]：	hItem：		 句柄
		@return		bool：            是返回true
		@author		丁显鹏
		@date		2014.10.15
		@remark
		*///}========================================================
		static bool IsFolder(PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		设置item打勾状态
		@param		[输入]：	hItem：		 句柄
		@param		[输入]：	bCheck：		 是否打勾
		@return		void
		@author		丁显鹏
		@date		2014.10.15
		@remark	    这个设置会改变所有子节点和父节点的状态
		*///}========================================================
		static void SetItemChecked(PSIHANDLE hItem, bool bCheck);

		//设置多个item打钩状态，这些items应该在一个根节点下
		//这样比每个item都调用SetItemChecked要快
		static void SetItemsChecked(const std::vector<PSIHANDLE>& items, bool bCheck);

		//===========================================================
		// brief :		获取hItem对应的节点所被选中的子节点个数
		// Returns:		int		被选中的自己点个数
		// Parameter:	hItem	待获取的Item节点
		// Author:    	jiaowei
		// DATE:      	2014.10.25 15:04:36
		//===========================================================
		static int GetSubCheckedCount(PSIHANDLE hItem);

		//===========================================================
		// brief :		获取hParent对应子节点PSIHANDLE
		// Returns:		PSIHANDLE		子节点PSIHANDLE
		// Parameter:	hParent		待获取PSIHANDLE的父节点
		//				index		父节点对应的索引
		// Author:    	jiaowei
		// DATE:      	2014.10.25 15:45:22
		//===========================================================
		static PSIHANDLE GetSubItemHandle(PSIHANDLE hParent, unsigned int index);

		/*{=====================================================*//**
		@brief		返回某个item的名称
		@param		[输入]：	hItem：      当前句柄
		@return     std::wstring ：      item的名称
		@author		丁显鹏
		@date		2014.10.27
		@remark		返回hItem的名称
		*///}========================================================
		static const wchar_t* GetItemName(const PSIHANDLE pHItem);

		/*{=====================================================*//**
		@brief		返回某个item的的全路径
		@param		[输入]：	hItem：      当前句柄
		@return     std::wstring ：      item的全路径
		@author		丁显鹏
		@date		2014.10.27
		@remark		返回hItem的全路径
		*///}========================================================
		static std::wstring GetItemPath(PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		返回某个item的扩展名
		@param		[输入]：	hItem：      当前句柄
		@return     std::wstring ：      item的扩展名
		@author		丁显鹏
		@date		2014.10.27
		@remark		返回文件的扩展名，扩展名不带"."
		*///}========================================================
		static const wchar_t* GetFileExtName(const PSIHANDLE phFile);

		//===========================================================
		// brief :		获取指定Item大小
		// Returns:		unsigned __int64 Item句柄文件的大小
		// Parameter:	hItem：      待获取的Item句柄
		// Author:    	jiaowei
		// DATE:      	2014.11.3 11:38:38
		//===========================================================
		static unsigned __int64 GetItemSize(PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		判断磁盘是否为SafeImager的取证U盘
		@param		[输入]：	hdevice：      磁盘句柄
		@return     bool                   是：true
		@author		丁显鹏
		@date		2014.11.11
		*///}========================================================
		static bool IsSafeImagerDisk(PSIHANDLE hDevice);

		static bool IsSafeImagerVolume(PSIHANDLE hVolume);

		/*{=====================================================*//**
		@brief		判断某个Item是否为删除的节点
		@param		[输入]：	hItem：      当前句柄
		@return		bool：        是删除：true
		@author		丁显鹏
		@date		2014.11.12
		@remark		对于卷，磁盘（返回false）
		*///}========================================================
		static bool IsItemDeleted(const PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		返回某个item的创建时间
		@param		[输入]：	hItem：   当前句柄
		@return     PPTIME：          时间
		@author		丁显鹏
		@date		2014.11.11
		@remark		只有文件或目录有时间属性，其他类型的Item则返回NULL
		*///}========================================================
		static PPTIME GetItemCreateTime(const PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		返回某个item的修改时间
		@param		[输入]：	hItem：   当前句柄
		@return     PPTIME：          时间
		@author		丁显鹏
		@date		2014.11.11
		@remark		只有文件或目录有时间属性，其他类型的Item则返回NULL
		*///}========================================================
		static PPTIME GetItemModfiyTime(const PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		返回某个item的最后访问时间
		@param		[输入]：	hItem：   当前句柄
		@return     PPTIME：          时间
		@author		丁显鹏
		@date		2014.11.11
		@remark		只有文件或目录有时间属性，其他类型的Item则返回NULL
		*///}========================================================
		static PPTIME GetItemLastAccessTime(const PSIHANDLE hItiem);

		// 某个节点下是否有目录节点
		static bool CanExpand(PSIHANDLE pItem);
		static int GetItemLevel(PSIHANDLE pItem);

		/*{=====================================================*//**
		@brief		获得所有子节点的数目(根据参数判断是否递归)
		@param		[输入]：	hItem：			句柄
		@param		[输入]：	bRecursion：	是否递归
		@param		[输入]：	expression: 过滤表达式
		@date		2018.7.30
		@remark	    深度递归计算所有子节点的数目
		*///}========================================================
		static void GetAllSubItems(PSIHANDLE hItem, std::vector<PSIHANDLE>& subItems,
			bool bRecursion = false, const std::wstring& expression = L"");

	private:
		/*{=====================================================*//**
		@brief		判断子节点是否全打勾
		@param		[输入]：	hItem：		 句柄
		@return		bool：            是返回true
		@author		丁显鹏
		@date		2014.10.15
		@remark	    没有子节点的item返回自身的打勾状态，UI层不应该调用此函数
		*///========================================================
		static bool IsSubAllChecked(PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		递归设置子节点打勾状态
		@param		[输入]：	hItem：		 句柄
		@param		[输入]：	bCheck：		 是否打勾
		@return		void
		@author		丁显鹏
		@date		2014.10.15
		@remark	    递归设置所有子节点的状态，UI层不应该调用此函数
		*///========================================================
		static void SetSubAllChecked(PSIHANDLE hItem, bool bCheck);

		//===========================================================
		// brief :		判断子节点是否为全部未打钩
		// Returns:		bool	是否为全未选
		// Parameter:	hItem：		 句柄
		// Author:    	jiaowei
		// DATE:      	2014.10.22 17:25:32
		//===========================================================
		static bool IsSubAllUnChecked(PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		通过路径得到文件项的句柄
		@param		[输入]：hParent：		主目录的句柄
		@param		[输入]：itemPath：	文件的路径
		@param		[输入]: CaseSensitive         是否区分大小写
		@return		PSIHANDLE：		文件项的句柄
		@author		张文攀
		@date		2015.4.1
		@remark
		*///}========================================================
		static PSIHANDLE GetItemHandleRecursive(PSIHANDLE hParent,
			const std::wstring itemPath, bool CaseSensitive = false);

	private:
		static std::map<std::wstring, std::wstring> volumeMountPaths_;
	};
}
#endif
