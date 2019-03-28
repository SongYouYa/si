#include "stdafx.h"
#include "./fileoper/PFileHandleHelper.h"
#include "filesystemshell/FileSystemShell.h"

using namespace filesystem;

namespace devtask
{
	CPFileHandleHelper::CPFileHandleHelper(devtask::ParentSubHandles& handles)
		: _handles(handles),
		_totalSize(0)
	{
	}

	void CPFileHandleHelper::update(const std::vector<PSIHANDLE>& rootHandles)
	{
		for (auto it = rootHandles.begin(); it != rootHandles.end(); ++it) {
			insert(*it);
		}
	}

	void CPFileHandleHelper::insert(PSIHANDLE parent)
	{
		std::vector<PSIHANDLE> children;
		CFileSystem::GetAllSubItems(parent, children);
		std::vector<PSIHANDLE> checkedChildren;
		for (auto it = children.begin(); it != children.end(); ++it) {
			ITEM_CHECK_STATE state = CFileSystem::ItemCheckState(*it);
			if (state != ITEM_CHECK_NONE) {
				checkedChildren.push_back(*it);
			}
		}
		if (!children.empty()) {
			_handles[parent] = checkedChildren;
		}
		for (auto it = checkedChildren.begin(); it != checkedChildren.end(); ++it) {
			PSIHANDLE childHandle = *it;
			ITEM_TYPE itemType = CFileSystem::GetItemType(childHandle);
			if (itemType == ITEM_TYPE_FILE) {
				uint64 size = CFileSystem::GetItemSize(childHandle);
				_totalSize += size;
			}
			else {
				insert(childHandle);
			}
		}
	}

	uint64 CPFileHandleHelper::getTotalSize()
	{
		return _totalSize;
	}
}
