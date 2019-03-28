#include "stdafx.h"
#include "PExportFileSizeCalculator.h"
#include "filesystemshell/FileSystemShell.h"
#include "ui/common/PFilePathChecker.h"
using namespace filesystem;
CPExportFileSizeCalculator::CPExportFileSizeCalculator()
	:_totalSize(0), _largestSingleSize(0)
{
}

void CPExportFileSizeCalculator::update(const std::vector<PSIHANDLE>& rootHandles)
{
	for (auto it = rootHandles.begin(); it != rootHandles.end(); ++it) {
		insert(*it);
	}
}

void  CPExportFileSizeCalculator::insert(PSIHANDLE parent)
{
	std::vector<PSIHANDLE> children;
	CFileSystem::GetAllSubItems(parent, children);
	for (auto it = children.begin(); it != children.end(); ++it) {
		PSIHANDLE childHandle = *it;
		ITEM_CHECK_STATE state = CFileSystem::ItemCheckState(childHandle);
		if (state == ITEM_CHECK_NONE) {
			continue;
		}
		ITEM_TYPE itemType = CFileSystem::GetItemType(childHandle);
		if (itemType == ITEM_TYPE_FILE) {
			uint64 size = CFileSystem::GetItemSize(childHandle);
			//在这里进行检查每一个导出文件的大小是否符合标准
			if (_largestSingleSize < size) {
				_largestSingleSize = size;
			}
			_totalSize += size;

		}
		else {
			insert(childHandle);
		}
	}
}

uint64 CPExportFileSizeCalculator::getTotalSize()
{
	return _totalSize;
}


uint64 CPExportFileSizeCalculator::getSingleSize()
{
	return _largestSingleSize;
}

