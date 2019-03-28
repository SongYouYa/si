#include "stdafx.h"
#include "PMemoryImageWidget.h"
#include "ui/common/widget/PWidgetTitleWrapper.h"
#include "ui/common/POperationButtonContainer.h"
#include "ui/common/POperationButton.h"
#include "ui/common/PFilePathChecker.h"

#include "uicommon/device/PDeviceUiUtils.h"
#include "libdevtask/PDeviceTaskUtils.h"
#include "sifile/PMemoryReader.h"
#include "utility/PSystemInfo.h"

#include "handler/PFsHandler.h"

using namespace devtask;
using namespace pdevice;
using namespace libdt;

extern CPTabManager* g_mainTab;
extern CPRecord* g_imageFolder;

CPMemoryImageWidget::CPMemoryImageWidget(QWidget* parent)
	:CPImageWidgetBase(PDF_RADIO | PDF_CUR_SELECT, parent)
{
#if 0
	//���ڲ���ʾ�ڴ���Ϣ�б���
	_sourceWrapper->setTitle(tr("physical memory"));
	_sourceTree->setMinimumHeight(60);
	_sourceTree->setColumnHidden(2, true);
	_sourceTree->setColumnHidden(4, true);
	_sourceTree->setColumnWidth(1, 150);
#else
	_sourceWrapper->hide();
	_centerLayout->addStretch(1);
#endif
}

void CPMemoryImageWidget::initWidget()
{
	CPImageWidgetBase::initCallback();
}

void CPMemoryImageWidget::showEvent(QShowEvent* e)
{
	CPImageWidgetBase::showEvent(e);
	initMemoryDisk();
}

void CPMemoryImageWidget::initMemoryDisk()
{
	if (_memoDisk.size > 0) {
		return;
	}

	siio::CPMemoryReader memoryReader;
	if (!memoryReader.open()) {
		return;
	}
	_memoDisk.size = memoryReader.size();
	memoryReader.close();

	_memoDisk.name = L"memory";

#if 0
	util::CPSysteminfo::getComputerName() + tr("physical memory").toStdWString();
#endif
	_memoDisk.displayName = L"memorydump";
		
	siio::CPMemoryReader::setDisplayName(_memoDisk.displayName);

	DeviceDisks disks;
	disks.push_back(_memoDisk);
	_sourceTree->setDevices(disks);
	_sourceTree->setDeviceSelected(_memoDisk.name, true);
}

void CPMemoryImageWidget::getDeviceParam(const libdt::OneDevice&oneDevice, libdt::TDeviceParam& devParam)
{
	devParam.deviceName = _memoDisk.name;
	devParam.deviceDisplayName = _memoDisk.displayName;
	devParam.deviceSize = _memoDisk.size;
}

void CPMemoryImageWidget::slotStartButtonClicked()
{
	//���ѡ�д��������Ͷ�д
	libdt::OneDevices sourceDevices;
	_sourceTree->getSelectedDevices(sourceDevices);
	if (sourceDevices.empty()) {
		CPMessageBox::warning(tr("Get memory device failed!"));
		return;
	}
	assert(sourceDevices.size() == 1);

	MemoryImageParam imageParam;
	_imageOptionWidget->getOptions(imageParam.formatOptions);
	_hashWidget->getHashOptions(imageParam.hashTypes);

	if (!checkTargetPath(imageParam.formatOptions.imageType,
		_memoDisk.size, imageParam.formatOptions.splitSize)) {
		return;
	}
	imageParam.imagePath = _filePathContainer->getPath();
	if (!checkDestExist(imageParam.imagePath)) {
		return;
	}

	if (!_beforeCase) {
		if (!CPCaseHandler::checkCaseCreated()) {
			return;
		}
	}

	//���о�������ʱ��Ŀ�����д�������������Զ�������������
	//��Ŀ����̲��ܽ��п�¡������
	libdt::OneDevices targetDevices;
	if (!CPFilePathChecker::getPathDevice(imageParam.imagePath, targetDevices)) {
		return;
	}

	//��������
	TaskParamPtr taskParam = std::make_shared<boost::any>(imageParam);
	CPTask* task = CPTaskController::get()->createTask(L"task.memoryimage", taskParam);

	CPDeviceTaskUtils::setWritable(targetDevices, false);
	task->addEndCallback([targetDevices](CPTask* t) {
		//������д
		CPDeviceTaskUtils::setWritable(targetDevices, true);
		g_imageFolder->removeFlag(RFlagNoShow);
	});

	CPRecord* record = task->rootRecord();
	record->setFolder(false);
	g_imageFolder->addChild(record);

	//task create callbacks
	for (auto& callback : _taskCallbacks) {
		callback(task);
	}

	CPTaskController::get()->doTask(task);

	emit signalStarted();
}

void CPMemoryImageWidget::addTaskCreateCallback(const TaskCallback& callback)
{
	_taskCallbacks.push_back(callback);
}
