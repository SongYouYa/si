#include "stdafx.h"
#include "PDirSelectWidget.h"

#include "./PWidgetTitleWrapper.h"

extern QRegExp VALID_FILENAME_REG; //限制非法字符
const int WIDGET_WIDTH_MAX = 600;

CPDirSelectWidget::CPDirSelectWidget(QWidget* parent)
	: CPPathSelectWidget(false, parent)
{
	layout()->setSpacing(0);
	_selectPathButton->setText(tr("select"));
	_selectPathButton->setCursor(Qt::PointingHandCursor);

	setMaximumWidth(WIDGET_WIDTH_MAX);
}


CPFileNameEdit::CPFileNameEdit(QWidget *parent)
	: QLineEdit(parent)
{
	setObjectName("filename_edit");
	QRegExpValidator *pReg = new QRegExpValidator(VALID_FILENAME_REG, this);
	this->setValidator(pReg);
	this->setMaxLength(30);
	setMaximumWidth(WIDGET_WIDTH_MAX);
}

CPFilePathContainer::CPFilePathContainer(QWidget* parent)
	: QWidget(parent)
{
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(5);

	QHBoxLayout* dirLayout = new QHBoxLayout();
	dirLayout->setMargin(0);
	_imageDirWidget = new CPDirSelectWidget();
	auto dirWrapper = new CPWidgetTitleWrapper(_imageDirWidget);
	dirWrapper->setTitle(tr("save dir"));
	dirLayout->addWidget(dirWrapper, 5);
	dirLayout->addStretch(1);

	QHBoxLayout* nameLayout = new QHBoxLayout();
	nameLayout->setMargin(0);
	_filenameEdit = new CPFileNameEdit();
	auto fileWrapper = new CPWidgetTitleWrapper(_filenameEdit);
	fileWrapper->setTitle(tr("filename"));
	nameLayout->addWidget(fileWrapper, 5);
	nameLayout->addStretch(1);

	layout->addLayout(dirLayout);
	layout->addLayout(nameLayout);
	setLayout(layout);
}

std::wstring CPFilePathContainer::getPath()
{
	std::wstring imagePath = _imageDirWidget->getPath().toStdWString() +
		L"/" + _filenameEdit->text().trimmed().toStdWString();
	CPPath::formatPathSlash(imagePath);
	return imagePath;
}