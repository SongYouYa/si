#include "stdafx.h"
#include "PFsFilterBar.h"

#include "base/PRecordFilterCfg.h"

#include "filesystemshell/PFileProperty.h"


CPFsFilterBar::CPFsFilterBar(QWidget* parent)
	: CPFilterBarWidget(parent),
	_appButton(nullptr)
{
	initLeftLayout();
	_exAppwidgets.push_back(_searchEdit);
	_exAppwidgets.push_back(_addButton);	
}

void CPFsFilterBar::initLeftLayout()
{
	_leftLayout->setSpacing(2);

	auto& filters = CPRecordFilterCfg::get()->getFilters(L"table.fsfile");
	for (auto &filter : filters){
		QString itemText = QString::fromStdWString(filter.display);
		QString itemExpr = QString::fromStdWString(filter.expr);
		QPushButton* button = new QPushButton();
		button->setCheckable(true);
		button->setObjectName(QString::fromStdWString(filter.name));
		button->setToolTip(itemText);
		_leftLayout->addWidget(button);
		std::wstring filterName = filter.name;
		connect(button, &QPushButton::toggled, this, [this, filterName](bool checked){
			if (filterName == L"app") {
				onAppFilterClicked(checked);
			}
			else {
				startFilte();
			}
		});
		_tool_exprs[button] = itemExpr;
		if (filterName != L"app") {
			_exAppwidgets.push_back(button);
		}
		else {
			_appButton = button;
		}
	}

	connect(this, SIGNAL(signalAppParsed()), this, SLOT(slotAppParsed()),
		Qt::QueuedConnection);
}

void CPFsFilterBar::onAppFilterClicked(bool checked)
{
	if (checked) {
		//禁用其他按钮
		if (_preCallback) {
			_preCallback();
			for (auto w : _exAppwidgets) {
				w->setDisabled(true);
			}
		}
		std::thread t([this](){
			filesystem::CPFileProperty::get()->parseAppFile();
			emit signalAppParsed();
		});
		t.detach();
	}
	else {
		//启用其他按钮
		for (auto w : _exAppwidgets) {
			w->setDisabled(false);
		}
		startFilte(true); //取消app过滤时，需要重新发送一遍
	}
}

void CPFsFilterBar::slotAppParsed()
{
	for (auto w : _exAppwidgets) {
		w->setDisabled(false);
	}
	if (_appButton && _appButton->isChecked()) {
		startFilte();
	}
}

void CPFsFilterBar::getToolExpr(QString& toolExpr)
{
	QStringList exprList;
	for (auto& one : _tool_exprs) {
		if (one.first->isChecked()) {
			exprList.push_back(one.second);
		}
	}
	toolExpr = exprList.join(" Or ");
}

void CPFsFilterBar::startFilte(bool force)
{
	QString lineText = _searchEdit->text();
	QString qlineExpr;
	if (!completeExpr(lineText, _recordType, qlineExpr)) {
		promptInvalid();
		return;
	}

	QString totalExpr;
	QString toolExpr;
	getToolExpr(toolExpr);
	if (!toolExpr.isEmpty()) {
		totalExpr += toolExpr;
	}
	if (!qlineExpr.isEmpty()) {
		if (!totalExpr.isEmpty()) {
			totalExpr += " and (" + qlineExpr + ")";
		}
		else {
			totalExpr += qlineExpr;
		}
	}

	changeExpr(totalExpr, force);
}