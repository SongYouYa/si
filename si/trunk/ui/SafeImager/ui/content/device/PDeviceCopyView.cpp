#include "stdafx.h"
#include "PDeviceCopyView.h"

#include "PDeviceTabNameDefs.h"
#include "ui/common/tab/PLevel2TabBar.h"

#include "PDeviceCloneWidget.h"
#include "PDeviceImageWidget.h"
#include "PMemoryImageWidget.h"

#include "ui/content/device/handler/PDeviceViewHandler.h"


CPDeviceCopyView::CPDeviceCopyView(QWidget* parent)
    : QWidget(parent),
      _memoryWidget(nullptr),
	  _deviceImageWidget(nullptr)
{
	setObjectName("copy_view");

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);

	CPLevel2TabBar* tabBar = new CPLevel2TabBar(":/styles/config/devicetab.json");
	layout->addWidget(tabBar);

	QStackedWidget* stackedWidget = new QStackedWidget();
	layout->addWidget(stackedWidget);

	CPTabManager* deviceTab = new CPTabManager(tabBar, stackedWidget);

	_deviceImageWidget = new CPDeviceImageWidget();
	deviceTab->addTab(PDEVICE_TAB_IMAGE, dtr(PDEVICE_TAB_IMAGE), _deviceImageWidget);

#if (defined OS_PWINDOWS) || (defined OS_PAPPLE)
	_memoryWidget = new CPMemoryImageWidget();
	deviceTab->addTab(PDEVICE_TAB_MEMORY, dtr(PDEVICE_TAB_MEMORY), _memoryWidget);
#endif

	auto cloneWidget = new CPDeviceCloneWidget();
	deviceTab->addTab(PDEVICE_TAB_CLONE, dtr(PDEVICE_TAB_CLONE), cloneWidget);

	setLayout(layout);

	CPDeviceViewHandler::get(cloneWidget, _deviceImageWidget)->updateDevices();

	deviceTab->navToTab(PDEVICE_TAB_IMAGE);
}

void CPDeviceCopyView::initView()
{
    if (_memoryWidget) {
        _memoryWidget->initWidget();
    }
	if (_deviceImageWidget) {
		_deviceImageWidget->initWidget();
	}
}
