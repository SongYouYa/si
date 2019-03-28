#pragma once

namespace qtutil
{
	class CPDeviceWatcher : public QObject
	{
		Q_OBJECT
			CPDeviceWatcher(QObject* parent = nullptr);
	public:
		static CPDeviceWatcher* get();

	signals:
		void signalDeviceChanged();

		private slots:
		void slotDeviceChanged();

	private:
		QTimer *_timer;

		static CPDeviceWatcher* _instance;
	};
}