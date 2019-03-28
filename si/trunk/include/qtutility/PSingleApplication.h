#pragma once

class QSharedMemory;
class QLocalServer;

namespace qtutil
{
	class CPSingleApplication : public QObject
	{
		Q_OBJECT
	public:
		CPSingleApplication(QObject* parent = 0);
		~CPSingleApplication();

		static void setAppIdentifier(const QString& identifier);
		bool IsRunning();

		private slots:
		//��������ʱ����
		void slotNewLocalConnection();

	private:
		//������ӳɹ�����˵���Ѿ�����ʵ��
		static bool tryConnect();
		void getLock();
		void releaseLock();
		//��ʼ����������, ������Ӳ���server���򴴽��������˳�
		void initLocalConnection();
		//���������
		void newLocalServer();

		bool _isRunning;
		QLocalServer *_localServer;
		QSharedMemory* _sharedMemory;

		static QString s_appIdentifier;
	};
}