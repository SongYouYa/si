#pragma once
#include <boost/signals2.hpp>

class CPNetLicHandler : public QObject
{
	Q_OBJECT
public:
	static CPNetLicHandler* get();
	~CPNetLicHandler();

	//申请授权
	int applyAuth(int port);
	bool isAuthed();

signals:
	//保持连接，需要创建一个线程，和UI线程通信，需要通过信号来中转下
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