#pragma once
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

class CPDeviceLoader : public QThread
{
	Q_OBJECT
	CPDeviceLoader();
public:
	static CPDeviceLoader* get();

	static void getAllDeviceHandles(std::vector<PSIHANDLE>& handles);
	void loadDevices(const std::vector<PSIHANDLE>& handles);
	//等于上面两个函数的和
	void loadAllDevices();

	//等待文件系统加载结束
	void waitForFinished();
	bool isFinished();

	void getLoadedDeviceHandles(std::vector<PSIHANDLE>& handles);
	void getCheckedDeviceHandles(std::vector<PSIHANDLE>& handles);

signals:
	void signalLoadStart();
	//加载设备完成或者失败，发送信号
	void signalLoadFinished(bool bSuc);

private:
	virtual void run();
	bool loadImp();

	bool _finished;
	std::vector<PSIHANDLE> _toloadDeviceHandles;
	std::vector<PSIHANDLE> _loadedDeviceHandles;

	boost::mutex _mutex; //加载文件系统锁
	boost::condition_variable_any _condtion;

	int _progress; //文件系统加载进度
	QTimer* _timer; //用于更新进度的计时器

	static CPDeviceLoader* _instance;
};