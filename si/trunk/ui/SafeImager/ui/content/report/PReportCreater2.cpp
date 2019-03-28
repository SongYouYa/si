#include "stdafx.h"
#include "PReportCreater2.h"
#include "report/PRecordReport2.h"



CPReportCreater2 *CPReportCreater2::_instance = nullptr;

CPReportCreater2::CPReportCreater2(QObject* parent)
	: CPReportCreaterBase(parent)
{
	
}

CPReportCreater2* CPReportCreater2::get(QObject* parent)
{
	if (!_instance) {
		_instance = new CPReportCreater2(parent);
	}
	return _instance;
}

std::wstring CPReportCreater2::doReportImpl(CPRecord* record, const std::wstring& outDir,
	bool withMedia)
{
	report::CPRecordReport2 recordReport(record);
	std::wstring htmlPath = recordReport.createReport(outDir);
	return htmlPath;
}


void CPReportCreater2::slotReportFinishedImpl(const std::wstring& htmlPath)
{
   
}


void CPReportCreater2::createReport(CPRecord* rootRecord)
{
	if (!isFinished()) {
		return;
	}

	doStartReport(rootRecord, false);
}
