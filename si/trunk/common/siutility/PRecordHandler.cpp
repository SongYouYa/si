#include "stdafx.h"
#include "siutility/PRecordHandler.h"


namespace siutil
{
	void CPRecordHandler::removeLastSameChilren(CPRecord* parent,
		const std::vector<std::wstring>& list)
	{
		std::vector<std::wstring> columnList;

		if (list.size() == 0) {
			//listΪ����Ƚ�������
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
		//���һ�в��Ƚ�
		for (size_t i = 0; i < rowCount - 1; ++i) {
			bool bSame = true; //��ͬ��ʶ��ʼ��
			//һ�еĲ����Ƚ�
			for (auto it = columnList.begin(); it != columnList.end(); ++it) {
				const std::wstring columnName = *it; //����

				CPRecord* lastRecord = parent->child(rowCount - 1);
				const CPVariant& lastVar = lastRecord->at(columnName);

				CPRecord* curRecord = parent->child(i);
				const CPVariant& curVar = lastRecord->at(columnName);

				if (lastVar != curVar) {
					//��ֵ����ͬ,�Ƚ���һ��
					bSame = false;
					break;
				}
			}

			if (bSame) {
				parent->removeChild(rowCount - 1); //ɾ�����һ��
				return; //��Ϊÿ�����ʱ,�������ͬ��,�������ֻ��һ����ͬ��,�ҵ���,���ؼ���
			}
		}
		return;
	}

	void CPRecordHandler::copyTree(CPRecord* srcRecord, CPRecord* dstRecord, int excFlags)
	{
		for (CPRecord* child : srcRecord->getChildren()) {
			if (!child->isFolder()) {
				//ֻ����Ŀ¼�ڵ�
				break;
			}
			if (child->flags() & excFlags) {
				continue;
			}
			CPRecord* newChild = child->getType()->newRecord();
			child->copyTo(newChild);
			dstRecord->addChild(newChild);
			//�ݹ鿽���ӽڵ�
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
		//�Ȼ�ȡ����Ŀ¼�ڵ�
		std::map<int, CPRecord*> srcRecords;
		getFolderRecords(srcRecord, srcRecords);
		std::map<int, CPRecord*> dstRecords;
		getFolderRecords(dstRecord, dstRecords);

		//����Դ�ڵ��ѡ�����������Ŀ��ڵ�ı�־
		for (auto itd : dstRecords) {
			CPRecord* drecord = itd.second;
			//���������Ŀ��ڵ��ָ����־
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
			//��Ŀ¼�ڵ㲻��Ҫ���
			if (!child->isFolder()) {
				return;
			}
			checkRecordByTypes(child, apps);
		}
	}
}