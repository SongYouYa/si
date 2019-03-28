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
	//���ڽ�������ʱ��Ҳ�������ɱ���
	//��ʱ���ɱ���0.5�룬���Ը��õĵȴ��������
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
#endif
	std::wstring htmlPath = doReportImpl(record, outDir, withMedia);

#ifdef OS_PUNIX
	//���ɱ������ջ���
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
		//����Ƕϵ����������ȡһ���µ�Ŀ¼����ֹ��֮ǰ�ı��渲��
		_reportDir = CPPath::getNewPath(_reportDir);
	}
#endif
	createReportThread(rootRecord, _reportDir, withMedia);
}