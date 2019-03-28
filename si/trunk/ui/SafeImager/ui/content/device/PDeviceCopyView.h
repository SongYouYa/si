#pragma once


//���ƽ���
class CPMemoryImageWidget;
class CPDeviceImageWidget;
class CPDeviceCopyView : public QWidget
{
	Q_OBJECT
public:
	CPDeviceCopyView(QWidget* parent = nullptr);

	void initView();

private:
	CPMemoryImageWidget* _memoryWidget;
	CPDeviceImageWidget* _deviceImageWidget;
};