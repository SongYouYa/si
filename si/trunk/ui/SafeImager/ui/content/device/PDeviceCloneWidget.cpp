#include "stdafx.h"
#include "PDeviceCloneWidget.h"
#include "ui/common/widget/PWidgetTitleWrapper.h"
#include "ui/common/POperationButtonContainer.h"
#include "ui/common/POperationButton.h"

#include "uicommon/device/PDeviceUiUtils.h"

#include "libdevtask/PDeviceTaskUtils.h"
#include "libdevtask/PTaskError.h"

#include "handler/PDeviceTaskHandler.h"

using namespace devtask;
using namespace libdt;
using namespace pdevice;

extern CPTabManager* g_mainTab;

CPDeviceCloneWidget::CPDeviceCloneWidget(QWidget* parent)
	:QWidget(parent)
{
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setMargin(0);

	QVBoxLayout* centerLayout = new QVBoxLayout();
	centerLayout->setSpacing(10);
	centerLayout->setContentsMargins(10, 5, 10, 10);

	QSplitter* splitter = new QSplitter();		//切分窗口
	splitter->setChildrenCollapsible(false);	//用户不能手动将子控件大小调为0.
	splitter->setOrientation(Qt::Vertical);		//垂直
	splitter->setHandleWidth(0);
	centerLayout->addWidget(splitter, 1);

	_sourceTree = new CPDeviceTreeWidget(PDF_USB | PDF_RADIO | PDF_CUR_SELECT, this);
	std::vector<int> widths = { 100, 200, 180 };
	_sourceTree->setColumnWidths(widths);
	auto sourceWrapper = new CPWidgetTitleWrapper(_sourceTree);
	sourceWrapper->setTitle(tr("source device"));
	splitter->addWidget(sourceWrapper);

	_usbDecorator = new CPDeviceUsbDecorator(sourceWrapper, this);

	_targetTree = new CPDeviceTreeWidget(PDF_USB | PDF_RADIO | PDF_CUR_SELECT, this);
	QWidget* spaceWrapper = new QWidget();
	QVBoxLayout* targetLayout = new QVBoxLayout(); 
	targetLayout->setContentsMargins(0, 10, 0, 0);
	auto targetWrapper = new CPWidgetTitleWrapper(_targetTree);
	targetWrapper->setTitle(tr("target device"));
	_eraseTailBox = new QCheckBox(tr("erase tail"));
	targetWrapper->addRightButton(_eraseTailBox);
	targetLayout->addWidget(targetWrapper);
	spaceWrapper->setLayout(targetLayout);
	splitter->addWidget(spaceWrapper);
#if 0
	_localDecorator = new CPDeviceLocalDecorator(targetWrapper, this);
#endif

	_hashWidget = new CPHashOptionWidget2();
	auto hashWrapper = new CPWidgetTitleWrapper(_hashWidget);
	hashWrapper->setTitle(tr("hash calc"));
	centerLayout->addWidget(hashWrapper);

	layout->addLayout(centerLayout);

	CPOperationButtonContainer* buttonContainer = new CPOperationButtonContainer();
	auto operButton = CPOperationButton::newForensicButton();
	buttonContainer->addRightWidget(operButton);
	layout->addWidget(buttonContainer);

	setLayout(layout);

	connect(operButton, SIGNAL(clicked()), this, SLOT(slotStartTask()));
}

void CPDeviceCloneWidget::setDevices(const pdevice::DeviceDisks& allDisks,
	const pdevice::DeviceDisks& localDisks,
	const pdevice::DeviceDisks& usbDisks)
{
	_usbDecorator->initView(allDisks, localDisks);
#if 0
	_localDecorator->initView(allDisks, usbDisks);
#else
	_targetTree->setDevices(usbDisks);
#endif
}

void CPDeviceCloneWidget::slotStartTask()
{
	if (!CPCaseHandler::checkCaseCreated()) {
		return;
	}

	DeviceCloneParam cloneParam;
	//检测选中磁盘数量和读写
	libdt::OneDevices sourceDevices;
	_sourceTree->getSelectedDevices(sourceDevices);
	if (sourceDevices.empty()) {
		CPMessageBox::warning(tr("Please select source devices."));
		return;
	}
	if (!CPDeviceUiUtils::checkReadable(sourceDevices)) {
		return;
	}
	assert(sourceDevices.size() == 1);
	cloneParam.sourceDevice = sourceDevices.at(0);

	libdt::OneDevices targetDevices;
	_targetTree->getSelectedDevices(targetDevices);
	if (targetDevices.empty()) {
		CPMessageBox::warning(tr("Please select target devices."));
		return;
	}
	if (!CPDeviceUiUtils::checkWritable(targetDevices)) {
		return;
	}
	assert(targetDevices.size() == 1);
	cloneParam.targetDevice = targetDevices.at(0);

	if (cloneParam.sourceDevice.deviceName == cloneParam.targetDevice.deviceName) {
		CPMessageBox::warning(tr("Source and target can not be the same!"));
		return;
	}

#if !defined(_DEBUG) || 1  //调试时，可以把1改为0，不进行判断
	uint64 sourceSize = CPDeviceTaskUtils::getTotalSize(sourceDevices);
	uint64 targetSize = CPDeviceTaskUtils::getTotalSize(targetDevices);
	if (!CPDeviceUiUtils::checkSrcTargetSize(sourceSize, targetSize)) {
		return;
	}
#endif

	if (CPDialog::Accepted != CPMessageBox::question(
		tr("clone will erase target disk, sure to continue?"))) {
		return;
	}

	DeviceDisk disk;
	CPDeviceManager::get()->getDiskByName(cloneParam.targetDevice.deviceName, disk);
	if (!disk.removable &&
		CPMessageBox::question(tr("The target device is local disk, are you sure to continue?"))
		!= CPDialog::Accepted) {
		return;
	}

	_sourceTree->selectAll(false);
	_targetTree->selectAll(false);

#if 0
	g_mainTab->navToTab(PTAB_TASK);//切换页面
#endif

	//获取任务参数
	_hashWidget->getHashOptions(cloneParam.hashTypes);
	if (cloneParam.hashTypes.empty()) {
		//为使断点续传的接口一致，这里添加一个空Hash计算
		cloneParam.hashTypes.push_back(siio::HASH_NONE);
	}
	cloneParam.eraseTail = _eraseTailBox->isChecked();

	CPTask* task = CPDeviceTaskHandler::createCloneTask(cloneParam);
	CPTaskController::get()->doTask(task);
}
