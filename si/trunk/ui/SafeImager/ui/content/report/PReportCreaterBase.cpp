#include "stdafx.h"
#include "PReportCreaterBase.h"


extern const std::wstring ReportDirName = L"report";

#ifdef OS_PUNIX
void synccache()
{
	//clear cache
	::system("sync");
}
#endif

CPReportCreaterBase::CPReportCreaterBase(QObject* parent)
	: QObject(parent), _finished(true)
{
	connect(this, &CPReportCreaterBase::signalReportFinished,
		this, &CPReportCreaterBase::slotReportFinished, Qt::QueuedConnection);
}

void CPReportCreaterBase::createReportThread(CPRecord* record, const std::wstring& outDir,
	bool withMedia)
{
	std::thread t(&CPReportCreaterBase::doReport, this, record,
		outDir, withMedia);
	t.detach();
}

void CPReportCreaterBase::doReport(CPRecord* record, const std::wstring& outDir,
	bool withMedia)
{
	_finished = false;
#if 0
	//现在进行任务时，也可以生成报告
	//延时生成报告0.5秒，可以更好的等待任务结束
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
#endif
	std::wstring htmlPath = doReportImpl(record, outDir, withMedia);

#ifdef OS_PUNIX
	//生成报告后，清空缓存
	synccache();
#endif

	emit signalReportFinished(htmlPath);
}

void CPReportCreaterBase::addOnceFinishCallback(const VoidCallback& callback)
{
	_onceFinishCallBacks.push_back(callback);
}

void CPReportCreaterBase::addStartCallback(const VoidCallback& callback)
{
	_startCallBacks.push_back(callback);
}

void CPReportCreaterBase::addEternalFinishCallback(const VoidCallback& callback)
{
	_eternalFinishCallBacks.push_back(callback);
}

void CPReportCreaterBase::slotReportFinished(const std::wstring& htmlPath)
{
	_htmlPath = htmlPath;
	slotReportFinishedImpl(htmlPath);

	for (auto& callback : _onceFinishCallBacks) {
		callback();
	}
	_onceFinishCallBacks.clear();
	for (auto& callback : _eternalFinishCallBacks) {
		callback();
	}
	_finished = true;
}

bool CPReportCreaterBase::isFinished()
{
	return _finished;
}

void CPReportCreaterBase::doStartReport(CPRecord* rootRecord, bool withMedia)
{
	for (auto& callback : _startCallBacks) {
		callback();
	}

	if (!rootRecord) {
		rootRecord = CPCase::get()->rootRecord();
	}
	_reportDir = CPCase::get()->caseDir() + L"/" + ReportDirName;
#if 0
	if (!CPCase::get()->isNew()) {
		//如果是断点续传，则获取一个新的目录，防止把之前的报告覆盖
		_reportDir = CPPath::getNewPath(_reportDir);
	}
#endif
	createReportThread(rootRecord, _reportDir, withMedia);
}