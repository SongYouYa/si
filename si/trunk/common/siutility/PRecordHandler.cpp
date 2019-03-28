#include "stdafx.h"
#include "siutility/PRecordHandler.h"


namespace siutil
{
	void CPRecordHandler::removeLastSameChilren(CPRecord* parent,
		const std::vector<std::wstring>& list)
	{
		std::vector<std::wstring> columnList;

		if (list.size() == 0) {
			//list为空则比较所有列
			CPRecordType* recordType = parent->getType();
			size_t columnCount = recordType->publicFieldCount();
			for (size_t i = 0; i < columnCount; ++i) {
				columnList.push_back(recordType->getPublicField(i)->name());
			}
		}
		else {
			columnList = list;
		}

		size_t rowCount = parent->childrenCount();
		//最后一行不比较
		for (size_t i = 0; i < rowCount - 1; ++i) {
			bool bSame = true; //相同标识初始化
			//一行的参数比较
			for (auto it = columnList.begin(); it != columnList.end(); ++it) {
				const std::wstring columnName = *it; //列名

				CPRecord* lastRecord = parent->child(rowCount - 1);
				const CPVariant& lastVar = lastRecord->at(columnName);

				CPRecord* curRecord = parent->child(i);
				const CPVariant& curVar = lastRecord->at(columnName);

				if (lastVar != curVar) {
					//有值不相同,比较下一行
					bSame = false;
					break;
				}
			}

			if (bSame) {
				parent->removeChild(rowCount - 1); //删除最后一行
				return; //因为每次添加时,都检查相同行,所以最多只会一行相同的,找到后,返回即可
			}
		}
		return;
	}

	void CPRecordHandler::copyTree(CPRecord* srcRecord, CPRecord* dstRecord, int excFlags)
	{
		for (CPRecord* child : srcRecord->getChildren()) {
			if (!child->isFolder()) {
				//只拷贝目录节点
				break;
			}
			if (child->flags() & excFlags) {
				continue;
			}
			CPRecord* newChild = child->getType()->newRecord();
			child->copyTo(newChild);
			dstRecord->addChild(newChild);
			//递归拷贝子节点
			copyTree(child, newChild, excFlags);
		}
	}

	void getFolderRecords(CPRecord* record, std::map<int, CPRecord*>& records)
	{
		for (CPRecord* child : record->getChildren()) {
			if (child->isFolder()) {
				records[child->id()] = child;
			}
			getFolderRecords(child, records);
		}
	}

	void CPRecordHandler::setCheckedFlags(CPRecord* srcRecord, CPRecord* dstRecord, int flags, bool add)
	{
		//先获取所有目录节点
		std::map<int, CPRecord*> srcRecords;
		getFolderRecords(srcRecord, srcRecords);
		std::map<int, CPRecord*> dstRecords;
		getFolderRecords(dstRecord, dstRecords);

		//根据源节点的选中情况，设置目标节点的标志
		for (auto itd : dstRecords) {
			CPRecord* drecord = itd.second;
			//先清除所有目标节点的指定标志
			if (add) {
				drecord->removeFlag(flags);
			}
			else {
				drecord->addFlag(flags);
			}
			auto its = srcRecords.find(drecord->id());
			if (its != srcRecords.end()) {
				CPRecord* srecord = its->second;
				if (srecord->getCheckState() != RECORD_CHECK_NONE) {
					if (add) {
						drecord->addFlag(flags);
					}
					else {
						drecord->removeFlag(flags);
					}
				}
			}
		}
	}

	void CPRecordHandler::checkRecordByTypes(CPRecord* record, const std::set<std::wstring>& apps)
	{
		std::wstring typeName = record->getType()->name();
		auto it = apps.find(typeName);
		if (it != apps.end()) {
			record->setChecked(true);
			return;
		}
		for (CPRecord* child : record->getChildren()) {
			//非目录节点不需要检查
			if (!child->isFolder()) {
				return;
			}
			checkRecordByTypes(child, apps);
		}
	}
}