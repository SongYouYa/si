#pragma once
//����ģʽ����¼��������Ĵ�����Ϣ��
/*
1 �����Ƿ����
2 ���淢���쳣�����¼�
3 ����û�����*/

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
