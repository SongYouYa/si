#include "stdafx.h"
#include "PExportReportDialog.h"
#include "uicommon/widget/PPathSelectWidget.h"
#include "application/DeviceTask/PDeviceTaskParam.h"

CPExportReportDialog::CPExportReportDialog(QWidget* parent)
: CPDialog2(parent),
_selectWidget(new CPDirSelectWidget(this))
{
	setWindowTitle(tr("export report"));

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

void CPExportReportDialog::setDir(const std::wstring& dir)
{
	_selectWidget->setPath(QString::fromStdWString(dir));
}

std::wstring CPExportReportDialog::getDir()
{
	return _selectWidget->getPath().toStdWString();
}