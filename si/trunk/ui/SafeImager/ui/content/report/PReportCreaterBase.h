#pragma once


class CPReportCreaterBase : public QObject
{
	Q_OBJECT
public:
	//加入的只执行一次，然后会清空
	void addOnceFinishCallback(const VoidCallback& callback);
	//生成报告时，界面进度回调
	void addStartCallback(const VoidCallback& callback);
	void addEternalFinishCallback(const VoidCallback& callback);

	std::wstring htmlPath() { return _htmlPath;	}
	std::wstring reportDir() { return _reportDir; }

	bool isFinished();

signals:
	void signalReportFinished(const std::wstring& htmlPath);

	protected slots:
	void slotReportFinished(const std::wstring& htmlPath);

protected:
	CPReportCreaterBase(QObject* parent);
	void doStartReport(CPRecord* rootRecord, bool withMedia);
	void createReportThread(CPRecord* record, const std::wstring& outDir, bool withMedia);
	void doReport(CPRecord* record, const std::wstring& outDir, bool withMedia);
	virtual std::wstring doReportImpl(CPRecord* record, const std::wstring& outDir,
		bool withMedia) = 0;
	
	virtual void slotReportFinishedImpl(const std::wstring& htmlPath) {};


	std::atomic<bool> _finished;
	std::wstring _htmlPath;
	std::wstring _reportDir;

	std::vector<VoidCallback>  _onceFinishCallBacks;
	std::vector<VoidCallback>  _startCallBacks;
	std::vector<VoidCallback>  _eternalFinishCallBacks;
};