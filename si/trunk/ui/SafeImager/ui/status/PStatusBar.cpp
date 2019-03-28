#include "stdafx.h"
#include "PStatusBar.h"
#include "base/PRecordHandler.h"

CPStatusBar::CPStatusBar(QWidget* parent)
	: QFrame(parent)
{
	setObjectName("status_bar");

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(10, 4, 10, 4);
	layout->setSpacing(5);

	QLabel* caseNameTitle = new QLabel(tr("case_name:"));
	_caseNameValue = new QLabel();
	QLabel* suspectTitle = new QLabel(tr("suspect_name:"));
	_suspectValue = new QLabel();

	layout->addWidget(caseNameTitle);
	layout->addWidget(_caseNameValue);
	layout->addSpacing(15);
	layout->addWidget(suspectTitle);
	layout->addWidget(_suspectValue);
	layout->addStretch(1);

	setLayout(layout);

	CPCase::get()->addCreateCallback([this](){
		updateCaseInfo();
	});
	CPCase::get()->addUpdateCallback([this](){
		updateCaseInfo();
	});
}

void CPStatusBar::updateCaseInfo()
{
	CPRecord* record = CPCase::get()->infoRecord();
	if (record) {
		std::wstring strName = CPRecordHandler::variantToString(record->at(L"case_name"));
		_caseNameValue->setText(QString::fromStdWString(strName));
		std::wstring strSuspect = CPRecordHandler::variantToString(record->at(L"case_suspect"));
		_suspectValue->setText(QString::fromStdWString(strSuspect));
	}
}