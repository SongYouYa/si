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
	//�����������������ĺ�
	void loadAllDevices();

	//�ȴ��ļ�ϵͳ���ؽ���
	void waitForFinished();
	bool isFinished();

	void getLoadedDeviceHandles(std::vector<PSIHANDLE>& handles);
	void getCheckedDeviceHandles(std::vector<PSIHANDLE>& handles);

signals:
	void signalLoadStart();
	//�����豸��ɻ���ʧ�ܣ������ź�
	void signalLoadFinished(bool bSuc);

private:
	virtual void run();
	bool loadImp();

	bool _finished;
	std::vector<PSIHANDLE> _toloadDeviceHandles;
	std::vector<PSIHANDLE> _loadedDeviceHandles;

	boost::mutex _mutex; //�����ļ�ϵͳ��
	boost::condition_variable_any _condtion;

	int _progress; //�ļ�ϵͳ���ؽ���
	QTimer* _timer; //���ڸ��½��ȵļ�ʱ��

	static CPDeviceLoader* _instance;
};