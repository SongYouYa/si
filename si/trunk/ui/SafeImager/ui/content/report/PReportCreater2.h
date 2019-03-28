#pragma once
#include "./PReportCreaterBase.h"

class CPReportCreater2 : public CPReportCreaterBase
{
	Q_OBJECT
public:
	static CPReportCreater2* get(QObject* parent = nullptr);
	
	void createReport(CPRecord* rootRecord = nullptr);

private:
	CPReportCreater2(QObject* parent);
	std::wstring doReportImpl(CPRecord* record, const std::wstring& outDir,
		bool withMedia) override;
	void slotReportFinishedImpl(const std::wstring& htmlPath) override;
	
	static 	CPReportCreater2 *_instance;
};