#pragma once
//单例模式，记录其他任务的处理信息：
/*
1 报告是否完成
2 报告发生异常更新事件
3 报告没有完成*/

class CPReportStatus : public QObject
{
	Q_OBJECT
public:
	enum ReportStaus
	{ 
		ReportUndone = 0, 
		ReportDone, 
		ReportUpdated 
	};
	static CPReportStatus *get(QObject* parent = nullptr);

	void setRecordDirty();
	void setReportCreated();
	int getStatus();

	void sendRecordChanged();

signals:
	void signalRecordChanged();

private:
	CPReportStatus(QObject* parent);

	static CPReportStatus *_instance;
	bool _created;
	bool _dirty;
};
