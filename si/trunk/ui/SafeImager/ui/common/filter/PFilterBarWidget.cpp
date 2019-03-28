#include "stdafx.h"
#include "PFilterBarWidget.h"

#include "base/PRecordTypeFactory.h"

#include "./dialog/PFilterExpDialog.h"

#include "filesystemshell/PFileProperty.h"


CPFilterBarWidget::CPFilterBarWidget(QWidget* parent)
	: CPFilterBarBase(parent)
{
	QHBoxLayout* containerLayout = new QHBoxLayout();
	containerLayout->setContentsMargins(0, 0, 0, 1);

	QFrame* container = new QFrame();
	container->setObjectName("filterbar_widget");
	containerLayout->addWidget(container);
	setLayout(containerLayout);

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(10, 6, 10, 6);

	_leftLayout = new QHBoxLayout();
	initLeftLayout();
	layout->addLayout(_leftLayout);

	layout->addStretch(1);

	_searchEdit = new CPSearchEdit();
	layout->addWidget(_searchEdit);

	_addButton = new QPushButton();
	_addButton->setObjectName("add_express_button");
	_addButton->setToolTip(tr("add filter"));

	layout->addWidget(_addButton);

	container->setLayout(layout);

	connect(_addButton, SIGNAL(clicked()), this, SLOT(slotOpenDialog()));
	connect(_searchEdit, SIGNAL(signalEnterPressed()), this, SLOT(slotEnterPressed()));
	connect(_searchEdit, SIGNAL(signalCancelClicked()), this, SLOT(slotCancelClicked()));
}

void CPFilterBarWidget::setType(const std::wstring& filterType)
{
	if (_strType == filterType) {
		return;
	}
	_strType = filterType;
	_recordType = CPRecordTypeFactory::get()->getType(_strType);
	_searchEdit->setText("");
	const CPRecordField* defaultField = _recordType->getDefaultFilterField();
	QString placeHolder = tr("default by") + " " + QString::fromStdWString(defaultField->display()) + " " + tr("filte");
	_searchEdit->setPlaceholderText(placeHolder);
}

void CPFilterBarWidget::slotOpenDialog()
{
	QString lineValue = _searchEdit->text().trimmed();
	QString expr;
	completeExpr(lineValue, _recordType, expr);

	filter::CPFilterExpDialog filterExpDialog(nullptr, _strType, expr.toStdWString());
	if (filterExpDialog.exec() == CPDialog::Accepted) {
		filterExpDialog.autoFillInfo();
		QString resultValue = filterExpDialog.getReslutFilterValue();;
		_searchEdit->setText(resultValue);
		startFilte();
	}
}

void CPFilterBarWidget::slotCancelClicked()
{
	startFilte();
}

void CPFilterBarWidget::slotEnterPressed()
{
	startFilte();
}

void CPFilterBarWidget::startFilte(bool force)
{
	QString lineText = _searchEdit->text();
	QString qlineExpr;
	if (!completeExpr(lineText, _recordType, qlineExpr)) {
		promptInvalid();
		return;
	}

	changeExpr(qlineExpr, force);
}