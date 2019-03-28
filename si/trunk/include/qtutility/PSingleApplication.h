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
		//有新连接时触发
		void slotNewLocalConnection();

	private:
		//如果连接成功，则说明已经存在实例
		static bool tryConnect();
		void getLock();
		void releaseLock();
		//初始化本地连接, 如果连接不上server，则创建，否则退出
		void initLocalConnection();
		//创建服务端
		void newLocalServer();

		bool _isRunning;
		QLocalServer *_localServer;
		QSharedMemory* _sharedMemory;

		static QString s_appIdentifier;
	};
}