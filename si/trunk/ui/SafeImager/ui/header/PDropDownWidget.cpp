#include "stdafx.h"
#include "PDropDownWidget.h"

#include "./PAboutDialog.h"


CPDropDownWidget::CPDropDownWidget(QWidget* parent)
	: QPushButton(parent)
{
	setObjectName("header_dropdown");
	setFixedSize(QSize(24, 24));

	/*添加下拉窗口，里面包含两个菜单栏*/
	_menu = new QMenu(this);
	_menu->setMinimumWidth(80);

	this->setMenu(_menu);

	initMenu();
}

void CPDropDownWidget::initMenu()
{
	QAction *helpAction = new QAction(_menu);
	helpAction->setText(tr("helpDoc"));
	//helpAction->setIcon(QPixmap(":/styles/icons/header/help.png"));

	QAction *aboutAction = new QAction(_menu);
	aboutAction->setText(tr("aboutInfo"));
	//aboutAction->setIcon(QPixmap(":/styles/icons/header/about.png"));

	QAction *urlAction = new QAction(_menu);
	urlAction->setText(tr("website"));

	_menu->addAction(aboutAction);
	_menu->addAction(helpAction);
	_menu->addAction(urlAction);
	
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(slotAbout()));
	connect(helpAction, SIGNAL(triggered()), this, SLOT(slotOpenHelp()));
	connect(urlAction, SIGNAL(triggered()), this, SLOT(slotUrl()));
}

void CPDropDownWidget::slotAbout()
{
	CPAboutDialog aboutDialog(this);
	aboutDialog.exec();
}

void CPDropDownWidget::slotOpenHelp()
{
	std::wstring helpDir = CPAppContext::get()->ipath(L"resource/doc");
	QString path = QString("file:///%1/help.chm").
		arg(QString::fromStdWString(helpDir));
	QUrl helpUrl(path);
	QDesktopServices desktopServices;
	desktopServices.openUrl(helpUrl);
}

void CPDropDownWidget::slotUrl()
{
	QDesktopServices::openUrl(QUrl(QLatin1String("https://www.pansafe.com")));
}