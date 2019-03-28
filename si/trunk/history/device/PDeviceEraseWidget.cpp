#include "stdafx.h"
#include "PDeviceEraseWidget.h"
#include "./material/PWidgetTitleWrapper.h"
#include "ui/common/POperationButtonContainer.h"
#include "ui/common/POperationButton.h"

#include "uicommon/device/PDeviceUiUtils.h"

#include "libdevtask/PDeviceTaskUtils.h"
#include "libdevtask/PTaskError.h"

using namespace devtask;
using namespace libdt;

extern CPTabManager* g_mainTab;
CPRecord* CPDeviceEraseWidget::s_eraseRecord = nullptr;


CPDeviceEraseWidget::CPDeviceEraseWidget(QWidget* parent)
	:QWidget(parent)
{
	setObjectName("content_container");

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setMargin(0);

	_targetTree = new CPDeviceTreeWidget(PDF_USB | PDF_RADIO | PDF_CUR_SELECT, this);
	auto targetWrapper = new CPWidgetTitleWrapper(_targetTree);
	targetWrapper->setTitle(tr("target device"));
	layout->addWidget(targetWrapper);
#if 0
	_localDecorator = new CPDeviceLocalDecorator(targetWrapper, this);
#endif

	QHBoxLayout* methodLayout = new QHBoxLayout();
	methodLayout->setContentsMargins(10, 5, 10, 10);
	methodLayout->setSpacing(30);
	QWidget* optionContainer = new QWidget();
	_eraseSimpleBtn = new QRadioButton(tr("simple"));
	_eraseBmbBtn = new QRadioButton("BMB");
	_eraseDodBtn = new QRadioButton("DOD");
	methodLayout->addWidget(_eraseSimpleBtn);
	methodLayout->addWidget(_eraseBmbBtn);
	methodLayout->addWidget(_eraseDodBtn);
	methodLayout->addStretch();
	optionContainer->setLayout(methodLayout);

	auto hashWrapper = new CPWidgetTitleWrapper(optionContainer);
	hashWrapper->setTitle(tr("erase method"));
	layout->addWidget(hashWrapper);

	CPOperationButtonContainer* buttonContainer = new CPOperationButtonContainer();
	CPToolButton::BtnConfig btnConfig;
	btnConfig.text = tr("erase");
	btnConfig.icon_normal = ":/styles/icons/device/start_erase.png";
	btnConfig.icon_hover = ":/styles/icons/device/start_erase_active.png";
	CPOperationButton* operButton = new CPOperationButton(btnConfig, parent);
	buttonContainer->addRightWidget(operButton);
	layout->addWidget(buttonContainer);

	setLayout(layout);

	_eraseSimpleBtn->setChecked(true);
	connect(operButton, SIGNAL(clicked()), this, SLOT(slotStartTask()));
}

void CPDeviceEraseWidget::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e);
	QStyleOption styleOpt;
	styleOpt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
}

void CPDeviceEraseWidget::initView(const pdevice::DeviceDisks& allDisks,
	const pdevice::DeviceDisks& usbDisks)
{
#if 0
	_localDecorator->initView(allDisks, usbDisks);
#else
	_targetTree->setDevices(usbDisks);
#endif
}

void CPDeviceEraseWidget::slotStartTask()
{
	if (!CPCaseHandler::checkCaseCreated()) {
		return;
	}

	DeviceEraseParam eraseParam;

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

    if (CPDialog::Accepted != CPMessageBox::question(
        tr("erase will erase target disk, sure to continue?"))) {
        return;
    }

	eraseParam.targetDevice = targetDevices.at(0);
	_targetTree->selectAll(false);

	if (_eraseSimpleBtn->isChecked()) {
		eraseParam.eraseType = ERASE_TYPE_SIMPLE;
	}
	else if (_eraseBmbBtn->isChecked()) {
		eraseParam.eraseType = ERASE_TYPE_BMB;
	}
	else if (_eraseDodBtn->isChecked()) {
		eraseParam.eraseType = ERADE_TYPE_DOD;
	}
	else {
		assert(false);
	}

	g_mainTab->navToTab(PTAB_TASK);//切换页面

	//创建任务
	TaskParamPtr taskParam = std::make_shared<boost::any>(eraseParam);
	CPTask* task = CPTaskController::get()->createTask(L"task.deviceerase", taskParam);

	//锁定读写
	CPDeviceTaskUtils::setRW(targetDevices, false);
	task->addEndCallback([targetDevices](CPTask* t) {
		//解锁读写
		CPDeviceTaskUtils::setRW(targetDevices, true);
	});

	CPRecord* record = task->rootRecord();
	if (!s_eraseRecord) {
		s_eraseRecord = record->getType()->newRecord();
		CPCase::get()->rootRecord()->addChild(s_eraseRecord);
	}
	record->setFolder(false);
	s_eraseRecord->addChild(record);
	
	CPTaskController::get()->doTask(task);
}
