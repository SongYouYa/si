#include "stdafx.h"
#include "PMainWindow.h"
#include "task/PTaskCfg.h"
#ifdef OS_PWINDOWS
#include "utility/PExceptionHandler.h"
#endif
#include "base/PRecordTypeFactory.h"
#include "base/PRecordFilterCfg.h"
#include "base/PRecordTreeCreater.h"
#include "ui/content/case/PCaseInfoDialog.h"

#include "qtutility/PSingleApplication.h"
#include "qtutility/PDeviceWatcher.h"

#include "assistant/PSiTaskCacheFactory.h"
#include "assistant/PSiTaskFactory.h"

#include "handler/PTaskCacheHandler.h"
#include "siutility/PDirHelper.h"
#include "uicommon/PGlobalSettings.h"
#include "uicommon/dialog/PMinmaxWidget.h"
#include "uicommon/dialog/PWindowDecorator.h"
#include "uicommon/tree/PCTreeItemWidgetBase.h"
#include "uicommon/table/PTableHeaderView.h"
#include "TemplateFile/PTemplateFile.h"

#include "ui/content/device/PMemoryImageDialog.h"
#include "ui/common/style/PCustomStyle.h"

#include "silicchecker/PSiLicChecker.h"
#include "PLicDefs.h"

#include "report/PReportConfig.h"
#include "handler/PCaseHandler.h"
#include "handler/PContextHandler.h"
#include "logic/PReportStatus.h"
#include "sifile/PMemoryReader.h"

#ifdef SI_VER2_CD
#include "ui/net/PIpDialog.h"
#include "ui/net/PNetLicenseDialog.h"
#include "ui/net/PNetLicHandler.h"
#endif

#include "qtutility/PAppUtil.h"
#include "logic/case/PCaseTypeHandler.h"
#include "logic/case/PCaseCatagoryHandler.h"

extern void test_device();

const std::wstring ConfName = L"conf";

void loadStyles(QApplication& a)
{
	//加载静态库中的资源，需要这句
	Q_INIT_RESOURCE(uicommon);

	std::wstring qtQmPath = CPAppContext::get()->ipath(L"resource/lang", L"qt_zh_CN.qm");
	QTranslator* qtTranslator = new QTranslator(&a);
	qtTranslator->load(QString::fromStdWString(qtQmPath));
	a.installTranslator(qtTranslator);

	std::wstring qmPath = CPAppContext::get()->ipath(L"resource/lang", L"zh_CN.qm");
	QTranslator* translator = new QTranslator(&a);
	translator->load(QString::fromStdWString(qmPath));
	a.installTranslator(translator);

	std::wstring uicQmPath = CPAppContext::get()->ipath(L"resource/lang", L"uic_zh_CN.qm");
	QTranslator* uicTranslator = new QTranslator(&a);
	uicTranslator->load(QString::fromStdWString(uicQmPath));
	a.installTranslator(uicTranslator);

	//qss
	QFile commonCssFile(":/puic/styles/css/uicommon.css");
	commonCssFile.open(QFile::ReadOnly);
	QString commonStyle = QString::fromLatin1(commonCssFile.readAll());
	commonCssFile.close();
	QFile file(":/styles/css/global.css");
	file.open(QFile::ReadOnly);
	QString styleSheet = QString::fromLatin1(file.readAll());
	file.close();
	styleSheet = commonStyle + styleSheet;
#ifdef OS_PAPPLE
	styleSheet += "* {font-family: Heiti SC; }";
#endif
#ifdef OS_PUNIX
	QFile linuxFile(":/styles/css/global_linux.css");
	linuxFile.open(QFile::ReadOnly);
	QString linuxStyleSheet = QString::fromLatin1(linuxFile.readAll());
	linuxFile.close();
    styleSheet += linuxStyleSheet;
#endif
	a.setStyleSheet(styleSheet);
}

void setOutDirs()
{
    CPAppContext* ctx = CPAppContext::get();
#ifdef OS_PWINDOWS
    std::wstring crashDir = ctx->opath(L"crash", false);
    std::wstring dumpFile = crashDir + L"/safeimager_" + ctx->version() + L"_"
        + CPUtils::getCurrentTimeString() + L".dmp";
    util::CPExceptionHandler::init(crashDir, dumpFile);
    util::CPExceptionHandler::addExceptionCallback([dumpFile]{
        QString msg = QObject::tr("The application has crashed, please send the dump file to software provider, thank you. \ndump file location: %1")
            .arg(QString::fromStdWString(dumpFile));
        CPMessageBox::info(msg);
    });
#endif
    std::wstring caseRootDir = ctx->opath(L"case");
	if (caseRootDir.empty()) {
		//一般如果输出目录是只读的话，就会创建目录失败
		CPMessageBox::error(QObject::tr("Create out dir failed!"));
		throw - 1;
	}
    CPCase::setCaseRootDir(caseRootDir);
    siutil::CPDirHelper::get()->setCaseRootDir(caseRootDir);
}

CPRecord* g_caseInfoFolder = nullptr;

void initCase()
{
	std::wstring caseDir = CPCase::get()->caseDir();
	std::wstring logName = CPCase::get()->caseNumber() + L".log";
	PLOG_OPEN(caseDir + L"/" + logName, PLOG_NOBUF);
	bool isNew = CPCase::get()->isNew();
	if (isNew) {
		PLOG(PLOG_CASE, L"新建案件");
	}
	else {
		PLOG(PLOG_CASE, L"打开案件");
	}
	CPTaskController::get()->setCacheFactory(new CPSiTaskCacheFactory(caseDir));
	//是否提示继续缓存任务
	CPTaskCacheHandler::handleCachedTasks(isNew);
	CPTaskController::get()->addCreateCallback([](CPTask* task, bool display){
		//有新的任务时，设置报告为数据已改变状态
		CPReportStatus::get()->setRecordDirty();
	});
#ifndef OS_PWINDOWS
	qApp->processEvents();
	//（windows下，这里不用更新）
	//在系统刚启动时，获取的磁盘分区挂载信息，可能已过时，
	//这里再重新更新下磁盘信息
	pdevice::CPDeviceManager::get()->update();
	qApp->processEvents();
#endif

	if (g_caseInfoFolder->flags() & RFlagNoShow) {
		g_caseInfoFolder->removeFlag(RFlagNoShow);

		CPReportStatus::get()->sendRecordChanged();
	}
}

void initCallbacks()
{
    CPAppContext* ctx = CPAppContext::get();
    ctx->addOutCallback(setOutDirs);
    CPCase::get()->addCreateCallback(initCase);
}

//第一阶段初始化，在内存镜像之前
bool init()
{
	PLOG_DEBUG(L"程序初始化");
	CPAppContext* ctx = CPAppContext::get();

    CPTaskController::get()->setTaskFactory(new CPSiTaskFactory);
	//同时执行任务最大数
	CPTaskController::get(4);
	
	std::wstring confDirPath = ctx->ipath(ConfName);
    siutil::CPDirHelper::get()->setConfDir(confDirPath);
	std::wstring toolPath = ctx->ipath(L"tool");
	siutil::CPDirHelper::get()->setToolDir(toolPath);

	std::wstring tableXml = ctx->ipath(ConfName, L"table.xml");
	if (!CPRecordTypeFactory::get()->loadXml(tableXml)) {
		PLOG_DEBUG(L"table.xml");
        return false;
    }

	std::wstring taskXml = ctx->ipath(ConfName, L"task.xml");
	CPTaskCfg::get(taskXml);

#ifdef OS_PAPPLE
    //用户自己设置加载驱动的路径
    std::wstring driverpath = toolPath + L"/macpmemdriver/MacPmem.kext";
    siio::CPMemoryReader::setDriverPath(driverpath);
#endif

	return true;
}

CPRecord* g_softwareFolder = nullptr;
CPRecord* g_appFolder = nullptr;
CPRecord* g_exportFileFolder = nullptr;
CPRecord* g_cloneFolder = nullptr;
CPRecord* g_imageFolder = nullptr;

void initRecords()
{
	CPRecordTypeFactory* factory = CPRecordTypeFactory::get();

	CPRecord* rootRecord = factory->getType(L"table.case")->newRecord();
	CPCase::get()->setRootRecord(rootRecord);

	g_softwareFolder = factory->getType(L"table.softwareinfo")->newRecord();
	rootRecord->addChild(g_softwareFolder);
	CPRecord* softwareRecord = g_softwareFolder->addChild();
	softwareRecord->at(L"name") = QObject::tr("SafeImager").toStdWString();
	softwareRecord->at(L"version") = CPAppContext::get()->version();
	softwareRecord->at(L"productor") = QObject::tr("pansafe").toStdWString();

	g_caseInfoFolder = CPCaseInfoParser::getCaseType()->newRecord();
	g_caseInfoFolder->addFlag(RFlagNoShow);
	rootRecord->addChild(g_caseInfoFolder);

	g_appFolder = factory->getType(L"table.approot")->newRecord();
	g_appFolder->addFlag(RFlagNoShow);
	rootRecord->addChild(g_appFolder);

	g_exportFileFolder = factory->getType(L"table.fileexportfolder")->newRecord();
	g_exportFileFolder->addFlag(RFlagNoShow);
	rootRecord->addChild(g_exportFileFolder);

	g_cloneFolder = factory->getType(L"table.deviceclone")->newRecord();
	g_cloneFolder->addFlag(RFlagNoShow);
	rootRecord->addChild(g_cloneFolder);

	g_imageFolder = factory->getType(L"table.deviceimage")->newRecord();
	g_imageFolder->addFlag(RFlagNoShow);
	rootRecord->addChild(g_imageFolder);

	CPRecord* signFolder = factory->getType(L"table.reportsign")->newRecord();
	rootRecord->addChild(signFolder);
}

//第二阶段初始化，在内存镜像之后
bool init2()
{
	CPAppContext* ctx = CPAppContext::get();

	auto devManager = pdevice::CPDeviceManager::get();
	//初始化
	//连接设备变化信号
	QObject::connect(qtutil::CPDeviceWatcher::get(), &qtutil::CPDeviceWatcher::signalDeviceChanged, []{
		//这里创建一个新的线程是因为，更新设备信息可能会阻塞，从而造成界面无响应
		new std::thread([](){
			pdevice::CPDeviceManager::get()->update();
			//离线下，由于有时会发生磁盘的外接属性识别错误的问题，
			//可能是因为磁盘在系统中的信息更新不及时，软件没有获mountDir取到最新的信息
			//所以这里延时5秒后，再次更新信息
			std::this_thread::sleep_for(std::chrono::seconds(5));
			pdevice::CPDeviceManager::get()->update();
		});
	});

	std::wstring filterXml = ctx->ipath(ConfName, L"filter.xml");
	if (!CPRecordFilterCfg::get()->loadXml(filterXml)) {
		PLOG_DEBUG(L"filter.xml");
		return false;
	}
	std::wstring winTableXml = ctx->ipath(ConfName, L"table_win.xml");
	if (!CPRecordTypeFactory::get()->loadXml(winTableXml)) {
		PLOG_DEBUG(L"table_win.xml");
		return false;
	}
	std::wstring macTableXml = ctx->ipath(ConfName, L"table_mac.xml");
	if (!CPRecordTypeFactory::get()->loadXml(macTableXml)) {
		PLOG_DEBUG(L"table_mac.xml");
		return false;
	}
#if 0
	//mac版需要加载mac增加的过滤项
	std::wstring versionFilterXml = ctx->ipath(confName, L"filter_mac.xml");
	if (!CPRecordFilterCfg::get()->loadXml(versionFilterXml)) {
		return false;
	}
#endif
	std::wstring appTreeXml = ctx->ipath(ConfName, L"tree.xml");
	if (!CPRecordTreeCreater::get()->loadXml(appTreeXml)) {
		PLOG_DEBUG(L"tree.xml");
		return false;
	}
	std::wstring templateDir = ctx->ipath(ConfName + L"/WinAppTemplate");
	tpfile::CPTemplateFile::setTemplateDir(templateDir);

	std::wstring reportDir = ctx->ipath(L"resource/report");
	report::SetConfRootDir(reportDir);
	std::wstring productName = (QObject::tr("360wangshen") + "(SafeImager)" +
		QObject::tr("scene forensic report")).toStdWString();
	report::SetProductName(productName);

	//case
	std::wstring caseTypePath = ctx->ipath(L"conf", L"CaseTypes.txt");
	if (!CPCaseTypeHandler::get()->loadTxt(caseTypePath)) {
		PLOG_DEBUG(L"CaseTypes.txt");
		return false;
	}
	std::wstring caseCataPath = ctx->ipath(L"conf", L"CaseCatagoryApps.ini");
	if (!CPCaseCatagoryHandler::get()->loadIni(caseCataPath)) {
		PLOG_DEBUG(L"CaseCatagoryApps.ini");
		return false;
	}

	return true;
}

void uiinit()
{
	//初始化界面配置
	CPGlobalSettings::get()->setWindowIconPath(":/styles/icons/logo_exe.ico");
	CPGlobalSettings::get()->setAppIconDir(":/styles/icons/apps/");
	CPGlobalSettings::get()->setLoadingMovie(":/styles/icons/progress/loading.gif", QSize(24, 24));
	//只保留最小的阴影
	CPWindow::setBorderColor(QColor(0, 0, 0, 1));
#ifdef OS_PUNIX
    //目前在ubuntu定制系统中，透明背景显示不正常，所以这里不使用透明背景
	CPWindow::setDefaultFlags(PWF_RADIUS);
#ifdef OS_PAPPLE
    //mac下使用系统的边框，这里设置自定义的边框为0
    CPWindow::setBorderWidth(0);
#elif OS_PLINUX
    CPWindow::setBorderWidth(3);
#endif
#else
	CPWindow::setDefaultFlags(PWF_BACKGROUND | PWF_RADIUS);
	CPWindow::setBorderWidth(10);
#endif
	
	CPWindowDecorator::setRadius(4);
	CPMinmaxWidget::setButtonSize(QSize(24, 24));
	CPNormalWindow::setTitleBarHeight(32);
	CPCTreeItemWidgetBase::setCheckBoxSize(QSize(14, 14));
	CPTreeItemWidgetBase::setIconSize(QSize(17, 17));
	CPTableHeaderView::psetAligment(Qt::AlignLeft | Qt::AlignVCenter);
}

void registerTypes()
{
	qRegisterMetaType<std::wstring>("std::wstring");
	qRegisterMetaType<size_t>("size_t");
	qRegisterMetaType<TASK_STATUS>("TASK_STATUS");
	qRegisterMetaType<TASK_WARNING>("TASK_WARNING");
}

bool checkLicense()
{
#ifdef SI_VER2_CD
	CPNetLicenseDialog netLicDialog;
	netLicDialog.exec();
	return CPNetLicHandler::get()->isAuthed();
#else
	int licError = 0;
	std::wstring licText;
	if (!lic::CPSiLicChecker::check(__DATE__, PPRODUCTID, PMODEL,
		PRELA_LIC_PATH, PRELA_CRT_PATH,
		licError, licText)) {
		CPMessageBox::error(QObject::tr("license error: ") +
			QString::fromStdWString(licText));
		return false;
	}
	return true;
#endif
    return false;
}


int main(int argc, char *argv[])
{
#if 0
#ifdef _DEBUG
	printf("please input a char:\n");
	char c;
	scanf("%c", &c);
#endif
#endif
	//没有安装Qt的环境中，需要添加plugin目录
    QString appDir = QString::fromStdWString(CPUtils::getAppDir());
    QApplication::addLibraryPath(appDir + "/plugins");
	QApplication a(argc, argv);
	a.setStyle(new CPCustomStyle());

#if defined(OS_PAPPLE) && !defined(_DEBUG) && 1
    if (!qtutil::CPAppUtil::appleIsAdmin(a, argc, argv)) {
        return 1;
    }
#endif

	loadStyles(a);
	uiinit();

	/*test_device();
	return 0;*/

#if 1
	qtutil::CPSingleApplication::setAppIdentifier("_pansafe_safeimager_singlenton_");
	qtutil::CPSingleApplication singleApp;
	if (singleApp.IsRunning()) {
		CPMessageBox::warning(QObject::tr("Application is already running!"));
		return 0;
	}
#endif

#ifdef SI_VER2_CD
#ifdef OS_PUNIX
    CPIpDialog ipDialog;
    ipDialog.exec();
#else
	CPMessageBox::info(QObject::tr("Windows platform for cd is not surpported yet!"));
	return 0;
#endif
#endif

#if 1
#if !defined(_DEBUG) || 0
	if (!checkLicense()) {
        //return 0;
	}
#endif
#endif

    initCallbacks();
	if (!CPContextHandler::initContext()) {
#ifndef SI_VER2_CD
		CPMessageBox::warning(QObject::tr("Get safeimager udisk failed."));
		//return false;
#endif
	}

    typedef boost::posix_time::microsec_clock boost_clock;
    auto startTime = boost_clock::local_time();
	QSplashScreen splash(QPixmap(":/styles/icons/splash.png"));
	splash.showMessage("                " + QObject::tr("app initing..."), 
		Qt::AlignVCenter | Qt::AlignLeft, Qt::white);

#ifndef SI_VER_LOGO_NO
    splash.show();
    splash.repaint();
#endif
	a.processEvents();

	registerTypes();

	if (!init()) {
        splash.finish(nullptr);
		CPMessageBox::error(QObject::tr("Init failed!"));
		return 0;
	}
	initRecords();

#if !defined(_DEBUG) || 0
#if defined(OS_PWINDOWS) || defined(OS_PAPPLE)
	{
		CPMemoryImageDialog memoryDialog;
		splash.finish(&memoryDialog);
		memoryDialog.exec();
	}
#endif
#endif

	splash.show();
	if (!init2()) {
		splash.finish(nullptr);
		CPMessageBox::error(QObject::tr("Init failed!"));
		return 0;
	}

	CPMainWindow w;

	//让启动画面显示至少1秒
    while ((boost_clock::local_time() - startTime).total_milliseconds() < 1000) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		a.processEvents();
	}
	w.show();
	splash.finish(&w);

    std::wstring caseRoot = CPCase::caseRootDir();
    if (!caseRoot.empty()) {
		CPCaseHandler::openCase(&w);
    }

	return a.exec();
}
