#pragma once
#include "./PReportCreaterBase.h"
#include "uicommon/progress/PWaitingDialog.h"

class CPReportCreater : public CPReportCreaterBase
{
	Q_OBJECT
public:
	static CPReportCreater* get(QObject* parent = nullptr);
	
	void createReport(CPRecord* rootRecord = nullptr);

private:
	CPReportCreater(QObject* parent);
	std::wstring doReportImpl(CPRecord* record, const std::wstring& outDir,
		bool withMedia) override;
	void slotReportFinishedImpl(const std::wstring& htmlPath) override;
	
	CPWaitingDialog* _waitingWidget;
	static 	CPReportCreater *_instance;
};