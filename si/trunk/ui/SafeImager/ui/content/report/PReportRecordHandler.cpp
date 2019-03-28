#include "stdafx.h"
#include "PReportRecordHandler.h"


void CPReportRecordHandler::getUnCheckedIds(CPRecord* record, std::set<int>& ids)
{
	if (record->getCheckState() == RECORD_CHECK_ALL) {
		return;
	}
	if (record->isFolder() &&
		record->getCheckState() == RECORD_CHECK_NONE) {
		ids.insert(record->id());
	}
	for (auto child : record->getChildren()) {
		if (!child->isFolder()) {
			break;
		}
		getUnCheckedIds(child, ids);
	}
}

void CPReportRecordHandler::uncheckAppsById(const std::set<int>& uncheckedIds, CPRecord* record)
{
	if (record->childrenCount() == 0) {
		//���һ��Ŀ¼�ڵ��ֱ��ѡ��
		int id = record->id();
		auto it = uncheckedIds.find(id);
		if (it != uncheckedIds.end()) {
			record->setChecked(false);
		}
	}
	else {
		for (auto child : record->getChildren()) {
			uncheckAppsById(uncheckedIds, child);
		}
	}
}
