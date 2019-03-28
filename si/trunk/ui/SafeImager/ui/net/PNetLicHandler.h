#pragma once
#include <boost/signals2.hpp>

class CPNetLicHandler : public QObject
{
	Q_OBJECT
public:
	static CPNetLicHandler* get();
	~CPNetLicHandler();

	//������Ȩ
	int applyAuth(int port);
	bool isAuthed();

signals:
	//�������ӣ���Ҫ����һ���̣߳���UI�߳�ͨ�ţ���Ҫͨ���ź�����ת��
	void signalKeepAlive();
	void signalAliveResult(bool ret);

	private slots:
	void slotKeepAlive();
	void slotAliveResult(bool ret);

private:
	CPNetLicHandler(QObject* parent = nullptr);
	void keepAliveProc();
		
	class Impl;
	Impl* _impl;
	bool _isAuthed;
	int _port;
	static CPNetLicHandler* _instance;
};