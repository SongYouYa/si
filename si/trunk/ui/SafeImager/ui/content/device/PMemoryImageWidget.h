#pragma once
#include "./PImageWidgetBase.h"

class CPMemoryImageWidget : public CPImageWidgetBase
{
	Q_OBJECT
public:
	CPMemoryImageWidget(QWidget* parent = nullptr);
	void initWidget();
	void addTaskCreateCallback(const TaskCallback& callback);

signals:
	void signalStarted();
	void signalCanceled();

	public slots:
	void slotStartButtonClicked() override;

protected:
	void showEvent(QShowEvent* e) override;

	void initMemoryDisk();
	void getDeviceParam(const libdt::OneDevice&oneDevice, libdt::TDeviceParam& devParam);

private:
	pdevice::DeviceDisk _memoDisk;
	std::vector<TaskCallback> _taskCallbacks;
};