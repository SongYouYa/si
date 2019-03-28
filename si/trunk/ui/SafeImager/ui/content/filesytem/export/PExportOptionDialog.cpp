#include "stdafx.h"
#include "PExportOptionDialog.h"
#include "uicommon/widget/PPathSelectWidget.h"
#include "application/DeviceTask/PDeviceTaskParam.h"

CPExportOptionDialog::CPExportOptionDialog(QWidget* parent)
: CPDialog2(parent),
_selectWidget(new CPDirSelectWidget(this))
{
	setWindowTitle(tr("export files"));

	QWidget* container = new QWidget(this);
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setMargin(15);
	_selectWidget->setFixedWidth(300);
	layout->addWidget(_selectWidget);
	container->setLayout(layout);
	this->setContentWidget(container);
	setupButtons();

	std::wstring caseDir = CPCase::get()->caseDir();
	_selectWidget->setDefaultDir(caseDir);
}

void CPExportOptionDialog::getExportParam(devtask::FileExportParam& exportParam)
{
	exportParam.exportDir = _selectWidget->getPath().toStdWString();
}