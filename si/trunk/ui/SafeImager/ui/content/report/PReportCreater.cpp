#include "stdafx.h"
#include "PReportCreater.h"
#include "report/PReportUtility.h"
#include "report/PRecordReport.h"

#include "logic/PReportStatus.h"
#include "controller/PCase.h"




CPReportCreater *CPReportCreater::_instance = nullptr;

CPReportCreater::CPReportCreater(QObject* parent)
: CPReportCreaterBase(parent), _waitingWidget(nullptr)
{
	
}

CPReportCreater* CPReportCreater::get(QObject* parent)
{
	if (!_instance) {
		_instance = new CPReportCreater(parent);
	}
	return _instance;
}

std::wstring CPReportCreater::doReportImpl(CPRecord* record, const std::wstring& outDir,
	bool withMedia)
{
	report::CPRecordReport recordReport(record);
	std::wstring htmlPath = recordReport.createReport(outDir, withMedia);
	return htmlPath;
}


void CPReportCreater::slotReportFinishedImpl(const std::wstring& htmlPath)
{
    CPReportStatus* status = CPReportStatus::get();
    status->setReportCreated(); //每次执行报告，生成报的状态标志位设置
	if (!htmlPath.empty()) {
		if (_waitingWidget) {
			_waitingWidget->close();
		}
#if defined(OS_PWINDOWS) || defined(OS_PAPPLE)
		int ret = CPMessageBox::question(tr("Report export success, open it with default browser?"));
		if (ret == CPDialog::Accepted) {
			bool ret = report::CPReportUtility::OpenHtml(htmlPath);
		}
#else
		CPMessageBox::info(tr("Report create success."));
#endif
	}
	else {
		CPMessageBox::error(tr("Report export failed!"));
	}
}


void CPReportCreater::createReport(CPRecord* rootRecord)
{
	if (!isFinished()) {
		return;
	}

	if (!CPCaseHandler::checkCaseCreated()) {
		return;
	}

	CPTaskController *controller = CPTaskController::get();  //获取任务状态
	bool allDone = controller->isAllTasksDone();
	if (!allDone) {
		int ret = CPMessageBox::question(tr("Some tasks are running, to cancel the tasks and create report?"));
		if (ret == CPDialog::Accepted) {
			controller->cancelAllTasks();
		}
		else {
			return;
		}
	}

	doStartReport(CPCase::get()->rootRecord(), true);
}
