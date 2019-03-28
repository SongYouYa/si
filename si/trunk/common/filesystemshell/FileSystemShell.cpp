#include "stdafx.h"
#include "filesystemshell/FileSystemShell.h"
#include "FuncTypedef.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#ifdef SI_OFFLINE
#include "system/linux/LinuxWMIHelper.h"
#endif

#include "expression/PExprParser.h"
#include "PFileFilter.h"
#include "base/PRecordTypeFactory.h"


namespace filesystem
{
	std::map<std::wstring, std::wstring> CFileSystem::volumeMountPaths_;

	/*{=====================================================*//**
	@brief		设置某个item的打勾状态
	@param		[输入]：	hItem：		        句柄
	@param		[输入]：	ITEM_CHECK_STATE：	状态
	@return		void：
	@author		丁显鹏
	@date		2014.10.15
	@remark		单独改变单个节点的打勾状态，UI层不应该调用此函数
	*///}========================================================
	inline void SetItemCheckState(PSIHANDLE hItem, ITEM_CHECK_STATE state)
	{
		return gpSetItemCheckStateFunc(hItem, state);
	}

    bool CFileSystem::Initialize(const std::wstring &dllDir)
	{
        return gLoadAllFunctions(dllDir);
	}

	int CFileSystem::LocalDeviceCount()
	{
		if (!gpGetDeviceCountFunc) {
			return 0;
		}
		return gpGetDeviceCountFunc();
	}

	PSIHANDLE CFileSystem::GetDeviceInfo(unsigned int index, SPDevice* pDevice)
	{
		return gpGetDeviceFunc(index, pDevice);
	}

	bool CFileSystem::GetDeviceInfo(const PSIHANDLE hDevice, SPDevice* pItem)
	{
		int deviceCount = LocalDeviceCount();
		for (int i = 0; i < deviceCount; i++) {
			PSIHANDLE hItem = GetDeviceInfo(i, NULL);
			if (hDevice == hItem) {
				GetDeviceInfo(i, pItem);
				return true;
			}
		}
		return false;
	}

	bool CFileSystem::LoadDevice(PSIHANDLE hDevice)
	{
		bool ret = gpParseDeviceFunc(hDevice);
#ifdef SI_OFFLINE
		if(ret) {
			std::vector<LinuxDiskPartition> disk_partitions;
			LinuxWMIHelper::GetInstance()->GetAllPartitionInformation(disk_partitions);

			int volumeCount = VolumeCount(hDevice);
			for(int i = 0; i < volumeCount; ++i)
			{
				PSIHANDLE pVolume = GetVolumeInfo(hDevice, i, NULL);
				std::wstring volumeName = GetItemName(pVolume);
				std::wstring newName = boost::replace_all_copy(volumeName, L"_0", L"");
				boost::replace_all(newName, L"_", L"");
				std::vector<LinuxDiskPartition>::iterator it = disk_partitions.begin();
				for(; it != disk_partitions.end(); ++it)
				{
					LinuxDiskPartition partition = *it;
					if(newName.find(partition.partition_name_) != std::string::npos)
					{
						volumeMountPaths_[volumeName] = partition.mount_point_; //替换卷名为实际挂载点
						break;
					}
				}
			}
		}
#endif
		return ret;
	}

	bool CFileSystem::ReleaseDevice(PSIHANDLE hDevice)
	{
		return gpReleaseDeviceFunc(hDevice);
	}

	void CFileSystem::ReleaseAllDevice()
	{
		return gpReleaseAllDeviceFunc();
	}

	bool CFileSystem::UpdateDeviceTimeZone(void *hDevice, long bias)
	{
		return gpUpdateDeviceTimeZoneFunc(hDevice, bias);
	}

	bool CFileSystem::UpdateVolumeTimeZone(void *hVolume, long bias)
	{
		return gpUpdateVolumeTimeZoneFunc(hVolume, bias);
	}

	ITEM_TYPE CFileSystem::GetItemType(const PSIHANDLE hItem)
	{
		return gpGetItemTypeFunc(hItem);
	}

	bool CFileSystem::IsUsbDevice(void *hDevice)
	{
		if (hDevice == NULL){
			return false;
		}
		int dc = gpGetUsbDeviceCountFunc();
		for (int i = 0; i < dc; i++) {
			if (hDevice == gpGetUsbDeviceFunc(i, NULL)) {
				return true;
			}
		}
		return false;
	}

	int CFileSystem::VolumeCount(const PSIHANDLE hDevice)
	{
		return gpGetVolumeCountFunc(hDevice);
	}

	int CFileSystem::LogicalVolumeCount()
	{
		return  gpGetLogicalVolumeCountFunc();
	}

	PSIHANDLE CFileSystem::GetVolumeInfo(const PSIHANDLE hDevice, int index, SPVolume* pVolume)
	{
		return gpGetVolumeFunc(hDevice, index, pVolume);
	}

	bool CFileSystem::GetVolumeInfo(const PSIHANDLE hVolume, SPVolume* pItem)
	{
		PSIHANDLE hParent = GetItemParent(hVolume);
		int count = VolumeCount(hParent);
		for (int i = 0; i < count; i++) {
			PSIHANDLE hItem = GetVolumeInfo(hParent, i, NULL);
			if (hItem == hVolume) {
				GetVolumeInfo(hParent, i, pItem);
				return true;
			}
		}

		int logicalVolumeCount = LogicalVolumeCount();
		for (int i = 0; i < logicalVolumeCount; i++) {
			PSIHANDLE hdl = GetLogicalVolumeInfo(i, NULL);
			if (hdl == hVolume) {
				GetLogicalVolumeInfo(i, pItem);
				return true;
			}
		}
		return false;
	}

	PSIHANDLE CFileSystem::GetLogicalVolumeInfo(unsigned int index, SPVolume* pItem)
	{
		return gpGetLogicalVolumeFunc(index, pItem);
	}

	int CFileSystem::SubItemCount(const PSIHANDLE hParent)
	{
		int count = 0;
		if (GetItemType(hParent) == ITEM_TYPE_DEVICE) {
			count = VolumeCount(hParent);
		}
		else {
			count = gpGetFileItemCountFunc(hParent);
		}

		if (count == -1) {
			return 0;
		}
		return count;
	}

	PSIHANDLE CFileSystem::GetFileItem(const PSIHANDLE hParent, unsigned int index, SPFileItem* pFileItem)
	{
		return gpGetFileItemFunc(hParent, index, pFileItem);
	}

	//目前离线下通过实际路径找不到handle
#ifdef OS_PWINDOWS
	PSIHANDLE CFileSystem::GetItemHandleByPath(const std::wstring& path)
	{
		if (path.empty())
		{
			return NULL;
		}

		std::wstring subItemPath = path;
		boost::replace_all(subItemPath, L"\\", L"/");

		std::wstring deviceName;
		std::wstring volumeName;
		std::string::size_type devicePos = subItemPath.find_first_of(L"/");
		std::string::size_type volumePos = subItemPath.find_first_of(L":");
		if (devicePos != std::string::npos
			&& volumePos != std::string::npos
			&& devicePos < volumePos)
		{
			deviceName = subItemPath.substr(0, devicePos);
			volumeName = subItemPath.substr(devicePos + 1, volumePos - devicePos);
			std::string::size_type subPos = subItemPath.find_first_of(L"/", volumePos);
			if (subPos != std::string::npos)  //如: disk 1/C:/XX
			{
				subItemPath = subItemPath.substr(subPos + 1);
			}
			else  //如: disk 1/C:
			{
				subItemPath.clear();
			}
		}
		else if (volumePos == std::string::npos)
		{
			volumeName.clear();
			if (devicePos == std::string::npos) //如: disk 1
			{
				deviceName = subItemPath;
			}
			else //如: disk 1/
			{
				deviceName = subItemPath.substr(0, devicePos);
			}
			subItemPath.clear();
		}
		else if (volumePos != std::string::npos)
		{
			deviceName.clear();
			volumeName = subItemPath.substr(0, volumePos + 1);
			if (devicePos == std::string::npos) //如: C:
			{
				subItemPath.clear();
			}
			else //如: C:/XX
			{
				subItemPath = subItemPath.substr(devicePos + 1);
			}
		}
		else
		{
			return NULL;
		}

		//在本地物理设备中查找卷
		std::wstring itemName;
		int nDevice = LocalDeviceCount();
		for (int i = 0; i < nDevice; ++i)
		{
			PSIHANDLE pDevice = GetDeviceInfo(i, NULL);
			if (IsSafeImagerDisk(pDevice))
			{
				continue;
			}
			itemName = GetItemName(pDevice);
			if (boost::iequals(itemName, deviceName) //不分大小写
				&& volumeName.empty()) //路径为设备
			{
				return pDevice;
			}

			int nVolume = VolumeCount(pDevice);
			for (int j = 0; j < nVolume; ++j)
			{
				PSIHANDLE pVolume = GetVolumeInfo(pDevice, j, NULL);
				if (CFileSystem::IsSafeImagerVolume(pVolume)){
					continue;
				}
				itemName = GetItemName(pVolume);
				if (boost::iequals(itemName, volumeName))
				{
					return GetItemHandleRecursive(pVolume, subItemPath); //查找卷下子目录
				}
			}
		}

		//本地没有，在逻辑卷中查找
		int nLogicalVolume = CFileSystem::LogicalVolumeCount();
		for (int i = 0; i < nLogicalVolume; ++i)
		{
			PSIHANDLE pLogicalVolume = CFileSystem::GetLogicalVolumeInfo(i, NULL);
			if (CFileSystem::IsSafeImagerVolume(pLogicalVolume)){
				continue;
			}
			itemName = GetItemName(pLogicalVolume);
			if (itemName == volumeName)
			{
				return GetItemHandleRecursive(pLogicalVolume, subItemPath);
			}
		}

		return NULL;
	}
#endif

	bool CFileSystem::GetItemHandlesByRawPath(const std::wstring& path, std::vector<PSIHANDLE>& handles, bool CaseSensitive)
	{
		//如果传的是绝对路径，则只返回指定卷的匹配handle
#ifdef OS_PWINDOWS
		if (boost::filesystem::exists(path)) {
			PSIHANDLE handle = GetItemHandleByPath(path);
			handles.push_back(handle);
			return true;
		}
#endif
		std::vector<PSIHANDLE> pVolumes;
		GetAllVolumes(pVolumes);

		std::vector<PSIHANDLE>::iterator vit = pVolumes.begin();
		for (; vit != pVolumes.end(); ++vit) {
			PSIHANDLE pVolume = *vit;
			std::vector<PSIHANDLE> tempHandles;
			GetItemHandlesByRawPath(pVolume, path, tempHandles, CaseSensitive);
			handles.insert(handles.end(), tempHandles.begin(), tempHandles.end());
		}

		return !handles.empty();
	}

	PSIHANDLE CFileSystem::GetItemHandleByRawPath(PSIHANDLE pRootHandle, const std::wstring& path, bool CaseSensitive)
	{
		if (path.empty()) {
			return NULL;
		}

		std::wstring itemPath = path;
		boost::replace_all(itemPath, L"\\", L"/");
		if (itemPath.at(0) == L'/') {
			itemPath = itemPath.substr(1); //去掉路径开始的'/'
		}

		PSIHANDLE handle = GetItemHandleRecursive(pRootHandle, itemPath, CaseSensitive); //查找卷下子目录
		return handle;
	}

	bool CFileSystem::GetItemHandlesByRawPath(PSIHANDLE pRootHandle, const std::wstring& path,
		std::vector<PSIHANDLE>& handles, bool CaseSensitive)
	{
		if (path.empty()) {
			return false;
		}

		std::wstring itemPath = path;
		boost::replace_all(itemPath, L"\\", L"/");
		if (!CaseSensitive) {
			boost::to_lower(itemPath);
		}

		std::vector<std::wstring> pathTokens;
		boost::split(pathTokens, itemPath, boost::is_any_of(L"/"));
		for (auto it = pathTokens.begin(); it != pathTokens.end();) {
			if (it->empty())
				it = pathTokens.erase(it); //删除空字符串
			else
				++it;
		}

		std::vector<PSIHANDLE> pMatchItems; //存放匹配的父项handle
		pMatchItems.push_back(pRootHandle);

		for (auto it = pathTokens.begin(); it != pathTokens.end(); ++it) {
			std::wstring itemName = *it;
			std::vector<std::wstring> nameTokens;
			boost::split(nameTokens, itemName, boost::is_any_of(L"*"), boost::token_compress_on);

			std::vector<PSIHANDLE> subItems; //存放待匹配的子项handle
			for (auto it_m = pMatchItems.begin(); it_m != pMatchItems.end(); ++it_m) {
				std::vector<PSIHANDLE> tempsubItems;
				GetAllSubItems(*it_m, tempsubItems, false);
				subItems.insert(subItems.end(), tempsubItems.begin(), tempsubItems.end());
			}
			pMatchItems.clear();

			for (auto it_s = subItems.begin(); it_s != subItems.end(); ++it_s) {
				std::wstring subName = GetItemName(*it_s);
				if (!CaseSensitive) {
					boost::to_lower(subName);
				}
				bool bMatch = true;
				std::wstring lastToken; //存放最后一个分割片段
				for (auto it_nt = nameTokens.begin(); it_nt != nameTokens.end(); ++it_nt) {
					std::wstring nametk = *it_nt;
					std::string::size_type pos = subName.find(nametk);
					if (pos == std::string::npos ||                          //没有找到，则不匹配
						pos > 0 && it_nt == nameTokens.begin()) {//第一个片段查找结果必须为0，才匹配
						bMatch = false;
						break;
					}
					else {
						subName = subName.substr(pos + nametk.size()); //剩余的字符串
					}
					lastToken = nametk;
				}
				if (!subName.empty() && !lastToken.empty()) {//剩余的字符串不为空时，则最后一个分割片段为空时，才匹配
					bMatch = false;
				}
				if (bMatch) {
					pMatchItems.push_back(*it_s);
				}
			}

			if (pMatchItems.empty()) {//没有找到匹配，则返回
				return false;
			}
		}

		handles.insert(handles.end(), pMatchItems.begin(), pMatchItems.end());
		return !handles.empty();
	}

	PSIHANDLE CFileSystem::GetItemHandleRecursive(PSIHANDLE hParent, std::wstring subItemPath, bool CaseSensitive)
	{
		if (subItemPath.empty()) {
			return hParent;
		}

		std::wstring itemName;
		std::wstring leftItemPath = subItemPath;
		std::string::size_type pos = leftItemPath.find_first_of(L"/");

		if (pos != std::string::npos) {
			itemName = leftItemPath.substr(0, pos);
			leftItemPath = leftItemPath.substr(pos + 1);
		}
		else {
			itemName = leftItemPath;
			leftItemPath.clear();
		}

		int itemCount = SubItemCount(hParent);
		for (int j = 0; j < itemCount; ++j) {
			PSIHANDLE pItem = GetSubItemHandle(hParent, j);

			if (CaseSensitive && boost::equals(itemName, GetItemName(pItem)) ||
				!CaseSensitive && boost::iequals(itemName, GetItemName(pItem))) {
				return GetItemHandleRecursive(pItem, leftItemPath);
			}
		}

		return NULL;
	}

	void CFileSystem::GetAllVolumes(std::vector<PSIHANDLE>& pVolumes)
	{
		int nDevice = LocalDeviceCount();
		for (int i = 0; i < nDevice; ++i)
		{
			PSIHANDLE pDevice = GetDeviceInfo(i, NULL);
			if (IsSafeImagerDisk(pDevice)) {
				continue;
			}
			int nVolume = VolumeCount(pDevice);
			for (int j = 0; j < nVolume; ++j) {
				PSIHANDLE pVolume = GetVolumeInfo(pDevice, j, NULL);
				if (CFileSystem::IsSafeImagerVolume(pVolume)){
					continue;
				}
				pVolumes.push_back(pVolume);
			}
#ifdef OS_PWINDOWS
			//逻辑卷
			int nLogicalVolume = CFileSystem::LogicalVolumeCount();
			for (int j = 0; j < nLogicalVolume; ++j) {
				PSIHANDLE pLogicalVolume = CFileSystem::GetLogicalVolumeInfo(j, NULL);
				if (CFileSystem::IsSafeImagerVolume(pLogicalVolume)){
					continue;
				}
				pVolumes.push_back(pLogicalVolume);
			}
#endif
		}
	}

	bool CFileSystem::InSameVolume(PSIHANDLE pItem1, PSIHANDLE pItem2)
	{
		ITEM_TYPE type1 = GetItemType(pItem1);
		ITEM_TYPE type2 = GetItemType(pItem2);
		if (type1 == ITEM_TYPE_DEVICE || type2 == ITEM_TYPE_DEVICE) {
			return false;
		}

		PSIHANDLE tempItem1 = pItem1;
		PSIHANDLE tempItem2 = pItem2;
		while (type1 != ITEM_TYPE_VOLUME) {
			tempItem1 = GetItemParent(tempItem1);
			type1 = GetItemType(tempItem1);
		}
		while (type2 != ITEM_TYPE_VOLUME) {
			tempItem2 = GetItemParent(tempItem2);
			type2 = GetItemType(tempItem2);
		}
		return tempItem1 == tempItem2;
	}

	bool CFileSystem::IsSubHandle(const PSIHANDLE pRootItem, PSIHANDLE pItem)
	{
		ITEM_TYPE type = GetItemType(pItem);
		if (type == ITEM_TYPE_DEVICE) {
			return false;
		}
		if (pRootItem == pItem) {//初始句柄相等，则不是子项
			return false;
		}

		PSIHANDLE tempItem = pItem;
		while (tempItem && tempItem != pRootItem) {
			tempItem = GetItemParent(tempItem);
		}

		if (pRootItem == tempItem) {
			return true;
		}
		return false;
	}

	bool CFileSystem::GetFileItemInfo(const PSIHANDLE hItem, SPFileItem* pItem)
	{
		return gpGetFileItemInfoFunc(hItem, pItem);
	}

	unsigned __int64 CFileSystem::ReadFile(const PSIHANDLE hFile,
		unsigned __int64 offset,
		unsigned __int64 length,
		unsigned char* buf)
	{
		return gpReadFileContentFunc(hFile, offset, length, buf);
	}

	ITEM_CHECK_STATE CFileSystem::ItemCheckState(const PSIHANDLE hItem)
	{
		return gpItemCheckStateFunc(hItem);
	}

	PSIHANDLE CFileSystem::GetItemParent(const PSIHANDLE hItem)
	{
		return gpGetItemParentFunc(hItem);
	}

	bool CFileSystem::IsFolder(PSIHANDLE hItem)
	{
		return GetItemType(hItem) != ITEM_TYPE_FILE;
	}

	bool CFileSystem::IsSubAllChecked(PSIHANDLE hItem)
	{
		int subCount = SubItemCount(hItem);
		if (subCount == 0) {
			ITEM_CHECK_STATE state = ItemCheckState(hItem);
			return (state == ITEM_CHECK_ALL) ? true : false;
		}

		for (int i = 0; i < subCount; i++) {
			PSIHANDLE hSubItem = GetSubItemHandle(hItem, i);
			if (ItemCheckState(hSubItem) != ITEM_CHECK_ALL) {
				return false;
			}
		}
		return true;
	}

	void CFileSystem::SetSubAllChecked(PSIHANDLE hItem, bool bCheck)
	{
		ITEM_CHECK_STATE state = bCheck ? ITEM_CHECK_ALL : ITEM_CHECK_NONE;
		int subCount = SubItemCount(hItem);
		for (int i = 0; i < subCount; ++i) {
			PSIHANDLE hSubItem = GetSubItemHandle(hItem, i);
			if (IsFolder(hSubItem)) {
				SetSubAllChecked(hSubItem, bCheck);
			}
			SetItemCheckState(hSubItem, state);
		}
	}

	void CFileSystem::SetItemChecked(PSIHANDLE hItem, bool bCheck)
	{
		ITEM_CHECK_STATE state = bCheck ? ITEM_CHECK_ALL : ITEM_CHECK_NONE;
		//1、自己打勾
		SetItemCheckState(hItem, state);
		//2、子节点全打勾
		SetSubAllChecked(hItem, bCheck);
		//3、父节点根据情况设置相应的状态
		PSIHANDLE hParent = GetItemParent(hItem);

		while (hParent) {
			if (IsSubAllChecked(hParent)) {
				SetItemCheckState(hParent, ITEM_CHECK_ALL);
			}
			else if (IsSubAllUnChecked(hParent)) {
				SetItemCheckState(hParent, ITEM_CHECK_NONE);
			}
			else {
				SetItemCheckState(hParent, ITEM_CHECK_HALF);
			}
			hParent = GetItemParent(hParent);
		}
	}

	bool CFileSystem::IsSubAllUnChecked(PSIHANDLE hItem)
	{
		int subCount = SubItemCount(hItem);

		if (0 == subCount) {
			ITEM_CHECK_STATE state = ItemCheckState(hItem);
			return (state == ITEM_CHECK_NONE) ? true : false;
		}

		for (int i = 0; i < subCount; i++) {
			PSIHANDLE hSubItem = GetSubItemHandle(hItem, i);
			if (ItemCheckState(hSubItem) != ITEM_CHECK_NONE) {
				return false;
			}
		}
		return true;
	}

	int CFileSystem::GetSubCheckedCount(PSIHANDLE hItem)
	{
		int count = 0;
		ITEM_CHECK_STATE item_state = ItemCheckState(hItem);

		if (ITEM_CHECK_NONE == item_state) {
			return 0;
		}

		int item_count = SubItemCount(hItem);
		if (ITEM_CHECK_ALL == item_state) {
			return item_count;
		}

		for (int i = 0; i < item_count; ++i) {
			PSIHANDLE file_handle = GetSubItemHandle(hItem, i);
			if (ITEM_CHECK_ALL == ItemCheckState(file_handle)) {
				count++;
			}
		}
		return count;
	}

	void updateParentState(PSIHANDLE item)
	{
		PSIHANDLE hParent = CFileSystem::GetItemParent(item);
		if (!hParent) {
			return;
		}
		ITEM_CHECK_STATE checkState = CFileSystem::ItemCheckState(item);
		if (checkState == ITEM_CHECK_HALF) {
			SetItemCheckState(hParent, ITEM_CHECK_HALF);
		}
		else {
			bool solved = false;
			int childrenCount = CFileSystem::SubItemCount(hParent);
			for (int i = 0; i < childrenCount; ++i) {
				PSIHANDLE child = CFileSystem::GetSubItemHandle(hParent, i);
				ITEM_CHECK_STATE childState = CFileSystem::ItemCheckState(child);
				if (childState == ITEM_CHECK_HALF ||
					childState != checkState) {
					SetItemCheckState(hParent, ITEM_CHECK_HALF);
					solved = true;
					break;
				}
			}
			if (!solved) {
				SetItemCheckState(hParent, checkState);
			}
		}

		updateParentState(hParent);
	}

	//根据父节点的打钩情况，更新子节点打钩
	void updateChildrenState(PSIHANDLE item)
	{
		ITEM_CHECK_STATE checkState = CFileSystem::ItemCheckState(item);
		int childrenCount = CFileSystem::SubItemCount(item);
		for (int i = 0; i < childrenCount; ++i) {
			PSIHANDLE child = CFileSystem::GetSubItemHandle(item, i);
			if (checkState != ITEM_CHECK_HALF) {
				SetItemCheckState(child, checkState);
			}
			if (CFileSystem::IsFolder(child)) {
				updateChildrenState(child);
			}
		}
	}

	void CFileSystem::SetItemsChecked(const std::vector<PSIHANDLE>& items, bool bCheck)
	{
		if (items.empty()) {
			return;
		}
		//std::vector<PSIHANDLE> rootHandles; //涉及的根节点，后面会从根节点遍历
		for (auto item : items) {
			SetItemCheckState(item, bCheck ? ITEM_CHECK_ALL : ITEM_CHECK_NONE);
			updateParentState(item);
		}
		//找到根节点，递归更新子节点的打勾
		PSIHANDLE hParent = items.at(0); 
		while (true) {
			PSIHANDLE tempParent = CFileSystem::GetItemParent(hParent);
			if (tempParent) {
				hParent = tempParent;
			}
			else {
				break;
			}
		}
		updateChildrenState(hParent);
	}

	PSIHANDLE CFileSystem::GetSubItemHandle(PSIHANDLE hParent, unsigned int index)
	{
		ITEM_TYPE type = GetItemType(hParent);

		if (type == ITEM_TYPE_DEVICE) {
			return GetVolumeInfo(hParent, index, NULL);
		}
		else if (type == ITEM_TYPE_VOLUME || type == ITEM_TYPE_DIRECTORTY) {
			return GetFileItem(hParent, index, NULL);
		}

		return NULL;
	}

	const wchar_t* CFileSystem::GetItemName(const PSIHANDLE hItem)
	{
		if (hItem)
		{
			wchar_t* pwstr;
			size_t len = (size_t)gpGetItemNameFunc(hItem, &pwstr);
			if (len) {
				return pwstr;
			}
		}

		static wchar_t p = L'\0';
		return &p;
	}

	std::wstring CFileSystem::GetItemPath(PSIHANDLE hItem)
	{
		if (hItem == NULL) {
			return L"";
		}

		std::wstring fullPath = GetItemName(hItem);
		if (GetItemType(hItem) == ITEM_TYPE_VOLUME)
		{
#ifdef OS_PUNIX
			std::wstring vname = GetItemName(hItem);
			if (volumeMountPaths_.find(vname) != volumeMountPaths_.end())
			{
				fullPath = volumeMountPaths_.at(vname).c_str(); //替换卷名为实际挂载点
			}
#elif OS_PWINDOWS
			fullPath = GetItemName(hItem); //在线不用替换
#endif
			return fullPath;
		}

		PSIHANDLE hNode = GetItemParent(hItem);
		while (hNode)
		{
			//卷下的本目录名称为“.” 去掉
			PSIHANDLE hParent = GetItemParent(hNode);

			const wchar_t* pstrName = NULL;
			if (hParent && GetItemType(hParent) == ITEM_TYPE_VOLUME)
			{
				hNode = hParent;
#ifdef OS_PUNIX
				std::wstring vname = GetItemName(hNode);
				if (volumeMountPaths_.find(vname) != volumeMountPaths_.end())
				{
					pstrName = volumeMountPaths_.at(vname).c_str(); //替换卷名为实际挂载点
				}
#elif OS_PWINDOWS
				pstrName = GetItemName(hNode); //在线不用替换
#endif
			}
			else {
				pstrName = GetItemName(hNode);
			}

			if (pstrName) {
				std::wstring parentName = pstrName;
				fullPath = parentName + L"/" + fullPath;
			}

			hNode = GetItemParent(hNode);

			if (hNode && GetItemType(hNode) == ITEM_TYPE_DEVICE) {//不加设备名
				break;
			}
		}

		return fullPath;
	}

	unsigned __int64 CFileSystem::GetItemSize(PSIHANDLE hItem)
	{
		return gpGetItemSizeFunc(hItem);
	}

	PPTIME CFileSystem::GetItemCreateTime(const PSIHANDLE hItem)
	{
		return gpGetItemCreateTimeFunc(hItem);
	}

	PPTIME CFileSystem::GetItemModfiyTime(const PSIHANDLE hItem)
	{
		return gpGetItemModifyTimeFunc(hItem);
	}

	const wchar_t* CFileSystem::GetFileExtName(const PSIHANDLE phFile)
	{
		if (ITEM_TYPE_FILE == GetItemType(phFile)) {
			wchar_t* pwstr;
			size_t len = (size_t)gpGetItemNameFunc(phFile, &pwstr);
			while (len > 0) {
				if (pwstr[len - 1] == L'.') return pwstr + len;

				len--;
			}
		}

		static wchar_t p = L'\0';
		return &p;
	}

	PPTIME CFileSystem::GetItemLastAccessTime(const PSIHANDLE hItem)
	{
		return gpGetItemLastAccessTimeFunc(hItem);
	}

	bool CFileSystem::CanExpand(PSIHANDLE pItem)
	{
		std::vector<PSIHANDLE> children;
		GetAllSubItems(pItem, children, false);
		auto it = children.begin();
		for (; it != children.end(); ++it) {
			ITEM_TYPE item_type = gpGetItemTypeFunc(*it);
			if (item_type != ITEM_TYPE_FILE && item_type != ITEM_TYPE_NONE) {
				return true;
			}
		}
		return false;
	}

	int CFileSystem::GetItemLevel(PSIHANDLE pItem)
	{
		int level = 0;
		PSIHANDLE parent = GetItemParent(pItem);
		while (parent) {
			++level;
			parent = GetItemParent(parent);
		}
		return level;
	}

	bool CFileSystem::IsItemDeleted(const PSIHANDLE hItem)
	{
		return gpIsItemDeletedFunc(hItem);
	}

	uint64 GetAllSubItemCountImpl(PSIHANDLE hItem, std::vector<PSIHANDLE>& subItems,
		size_t subCount, bool bRecursion, CPRecordType* recordType, const RecordCondss& condss)
	{
		uint64 totalCount = 0;
		for (size_t i = 0; i < subCount; i++) {
			PSIHANDLE hSubItem = CFileSystem::GetSubItemHandle(hItem, i);
			if (CPFileFilter::isMatch(hSubItem, recordType, condss)) {
				++totalCount;
				subItems.push_back(hSubItem);
			}
			if (bRecursion && CFileSystem::IsFolder(hSubItem)) {
				size_t sub2Count = (size_t)CFileSystem::SubItemCount(hSubItem);
				totalCount += GetAllSubItemCountImpl(hSubItem, subItems, sub2Count, bRecursion, recordType, condss);
			}
		}
		return totalCount;
	}

	void CFileSystem::GetAllSubItems(PSIHANDLE hItem, std::vector<PSIHANDLE>& subItems,
		bool bRecursion, const std::wstring& expression)
	{
		//std::clock_t st = std::clock();
		CPRecordType* recordType = CPRecordTypeFactory::get()->getType(L"table.fsfile");
		RecordCondss recordCondss;
		if (!CPRecordFilter::getRecordCondss(recordType, expression, recordCondss)) {
			return;
		}
		size_t subCount = (size_t)CFileSystem::SubItemCount(hItem);
		uint64 totalCount = GetAllSubItemCountImpl(hItem, subItems, subCount, bRecursion, recordType, recordCondss);
		//std::clock_t et = std::clock();
		//std::clock_t p = et - st;
		//std::cout << "fs time: " << p << std::endl;
	}

	bool CFileSystem::IsSafeImagerDisk(PSIHANDLE hDevice)
	{
		if (GetItemType(hDevice) == ITEM_TYPE_DEVICE) {
			SPDevice device;
			GetDeviceInfo(hDevice, &device);
			std::wstring strManufacture = device.strManufacture;
			//toupper
			std::transform(strManufacture.begin(),
				strManufacture.end(),
				strManufacture.begin(),
				towupper);

			if (strManufacture.find(L"PANSAFE") != std::wstring::npos) {
				return true;
			}
		}

		return false;
	}

	bool CFileSystem::IsSafeImagerVolume(PSIHANDLE hVolume)
	{
		if (!hVolume) {
			return false;
		}
		SPVolume volumeInfo;
		if (!GetVolumeInfo(hVolume, &volumeInfo)) {
			return false;
		}
		std::wstring name = volumeInfo.strVolumeName;
		std::transform(name.begin(), name.end(),
			name.begin(), towupper);
		if (name.find(L"SAFEIMAGER") != std::wstring::npos){
			return true;
		}
		return false;
	}
}
