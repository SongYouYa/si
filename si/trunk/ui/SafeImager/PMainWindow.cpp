#include "stdafx.h"
#include "PMainWindow.h"
#include "ui/content/apps/PAppView.h"
#include "ui/content/device/PDeviceCopyView.h"
#include "ui/content/filesytem/PFilesystemView.h"
#include "ui/content/task/PTaskView.h"
#include "ui/content/report/PReportView.h"
#include "uicommon/PGlobalSettings.h"
#include "ui/header/PHeaderWidget.h"
#include "ui/header/PTabBarWidget.h"

#include "ui/content/report/PReportCreater2.h"
#include "ui/status/PStatusBar.h"

CPTabManager* g_mainTab = nullptr;

#ifdef OS_PLINUX
void shutdownComputer()
{
	//poweroff命令 可能会造成导出的文件或报告没有写到磁盘上
	//所以改用下列命令
	::system("sync && shutdown -h now"); //shutdown -h now
}
#endif


CPMainWindow::CPMainWindow(QWidget *parent)
	: CPWindow(parent),
	_appView(nullptr)
{
	CPGlobalSettings::get()->setMainWidow(this);
	this->setWindowIcon(QIcon(CPGlobalSettings::get()->getWindowIconPath()));

	CPWindow::initWindow();
	
	//关闭主窗口时，即使有其他窗口，程序也会关闭
	setAttribute(Qt::WA_QuitOnClose, true);
#ifndef OS_PAPPLE
    this->setWindowTitle(tr("SafeImager"));
#endif
	this->resize(QSize(1100, 700)); //1050,720
	setMinimumWidth(650);

	createNavs();
	createTabConents();

#ifdef OS_PUNIX
    //linux系统下，界面没有居中显示，这里把主界面移到桌面中间位置
    move((QApplication::desktop()->width() - width()) / 2,
        (QApplication::desktop()->height() - height()) / 2);
#endif
	initContents();
}

void CPMainWindow::createNavs()
{	
	CPHeaderWidget* header = new CPHeaderWidget();
	header->setMainWindow(this);
	_containerLayout->addWidget(header);

	QStackedWidget* stackedWidget = new QStackedWidget();//多页面切换--
	_containerLayout->addWidget(stackedWidget);

	//status
	CPStatusBar* statusBar = new CPStatusBar();
	_containerLayout->addWidget(statusBar);

	g_mainTab = new CPTabManager(header->tabBar(), stackedWidget);
}

void CPMainWindow::createTabConents()
{
#ifndef OS_PAPPLE
	_appView = new CPAppView();
	g_mainTab->addTab(PTAB_APP, dtr(PTAB_APP), _appView);

	auto fsView = new CPFilesystemView();
	g_mainTab->addTab(PTAB_FILESYSTEM, dtr(PTAB_FILESYSTEM), fsView);
#endif
	_copyView = new CPDeviceCopyView();
	g_mainTab->addTab(PTAB_COPY, dtr(PTAB_COPY), _copyView);

	auto reportView = new CPReportView();
	g_mainTab->addTab(PTAB_REPORT, dtr(PTAB_REPORT), reportView);

	CPTaskView* taskView = new CPTaskView();
	QWidget* taskTab = g_mainTab->addTab(PTAB_TASK, dtr(PTAB_TASK), taskView);
	taskView->decorateTab(taskTab);

	g_mainTab->endArea();
#ifdef OS_PAPPLE
    g_mainTab->navToTab(PTAB_COPY);
#else
	g_mainTab->navToTab(PTAB_APP);
#endif
}

void CPMainWindow::initContents()
{
	if (_appView) {
		_appView->initView();
	}
	if (_copyView) {
		_copyView->initView();
	}
}

void CPMainWindow::closeEvent(QCloseEvent * event)
{
	if (CPCase::get()->caseNumber().empty()) {
		event->accept();
		return;
	}

	if (!CPReportCreater2::get(this)->isFinished()) {
		CPMessageBox::warning(tr("creating report, please wait."));
		event->ignore();
		return;
	}

	CPTaskController *controller = CPTaskController::get();  //获取任务状态
	bool allDone = controller->isAllTasksDone();
	if (!allDone) {
		int ret = CPMessageBox::question(tr("Some tasks are running, sure to close the program?"));
		if (ret == CPDialog::Accepted) {
			controller->cancelAllTasks();
			event->accept();
		}
		else {
			event->ignore();
		}
		return;
	}

	int ret = CPMessageBox::question(tr("Sure to close the program?"));
	if (ret == CPDialog::Accepted) {
		event->accept();
		return;
	}
	event->ignore();
}
