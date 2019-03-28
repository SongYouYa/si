#pragma once


class CPReportRecordHandler
{
public:
	static void getUnCheckedIds(CPRecord* record, std::set<int>& ids);
	static void uncheckAppsById(const std::set<int>& uncheckedIds, CPRecord* record);
};