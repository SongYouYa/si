#include "stdafx.h"
#include "PToolsHandler.h"

#include "base/PRecordTypeFactory.h"
#include "handler/PContextHandler.h"


static CPRecord* s_photoRecord = nullptr;
static CPRecord* s_videoRecord = nullptr;

CPToolsHandler::CPToolsHandler(QObject* parent)
	: QObject(parent),
	_photoTimer(nullptr), _videoButton(nullptr),
	_mainWindow(nullptr), _videoRunning(false),
	_videoMovie(nullptr), _videoTask(nullptr)
{

}

void CPToolsHandler::setCaseButton(QToolButton* button)
{
	connect(button, SIGNAL(clicked()), this, SLOT(slotCaseClicked()));
}

void CPToolsHandler::setCmdButton(QToolButton* button)
{
	connect(button, SIGNAL(clicked()), this, SLOT(slotCmdClicked()));
}

void CPToolsHandler::setPhotoButton(QToolButton* button)
{
	_photoTimer = new QTimer(this);
	_photoTimer->setInterval(1000);
	_photoTimer->setSingleShot(true);
	connect(_photoTimer, SIGNAL(timeout()), this, SLOT(slotOnScreenTimeout()));
	connect(button, SIGNAL(clicked()), this, SLOT(slotPhotoClicked()));
}

void CPToolsHandler::setVideoButton(QToolButton* button)
{
	_videoButton = button;
	connect(button, SIGNAL(clicked()), this, SLOT(slotVideoClicked()));
	_videoMovie = new QMovie(":/styles/icons/header/video.gif", QByteArray(), this);
	connect(_videoMovie, &QMovie::frameChanged, this, [this](int frame){
		_videoButton->setIcon(_videoMovie->currentPixmap());
	});
}

void CPToolsHandler::slotCaseClicked()
{
	if (CPCase::caseRootDir().empty()) {
		CPMessageBox::info(tr("Please set out dir first."));
		CPContextHandler::initContext();
		if (CPCase::caseRootDir().empty()) {
			return;
		}
	}
	CPCaseHandler::openCase(nullptr);
}

void CPToolsHandler::slotPhotoClicked()
{
	if (!CPCaseHandler::checkCaseCreated()) {
		return;
	}
	_mainWindow->hide();
	_photoTimer->start();
}

void CPToolsHandler::slotOnScreenTimeout()
{
	std::wstring captureDir = CPCase::get()->caseDir() + L"/CaptureFile";
	CPPath::createDirs(captureDir);

	QPixmap pixmap = QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId());
	//显示主对话框
	_mainWindow->show();
	std::wstring file_path = captureDir + L"/CaptureFile.png";
	file_path = CPPath::getNewPath(file_path);

	if (!s_photoRecord) {
		s_photoRecord = CPRecordTypeFactory::get()->getType(L"table.screenshot")->newRecord();
		CPCase::get()->rootRecord()->addChild(s_photoRecord);
	}
	CPRecord* record = s_photoRecord->addChild();
	record->at(L"savePath") = file_path;
	record->at(L"curTime") = CPTime::GetCurrentTime();

	if (!pixmap.save(QString::fromStdWString(file_path))){
		CPMessageBox::warning(tr("save screen failed!"));
		return;
	}
}

void CPToolsHandler::slotVideoClicked()
{
	if (!CPCaseHandler::checkCaseCreated()) {
		return;
	}

	if (!_videoRunning){
		_videoRunning = true;
		_videoMovie->start();
		std::wstring videoDir = CPCase::get()->caseDir() + L"/VideoFile";
		CPPath::createDirs(videoDir);
		std::wstring videoFilePath = videoDir + L"/video.wmv";
		videoFilePath = CPPath::getNewPath(videoFilePath);
		std::wstring videoExePath = siutil::CPDirHelper::get()->getToolDir() + L"/video/";
#ifdef OS_PWINDOWS
		videoExePath += L"ScreenRecorder.exe";
#elif __linux__
		videoExePath += L"linux/ScreenRecorder";
#elif OS_PAPPLE
		videoExePath += L"mac/ScreenRecorder.app/Contents/MacOS/ScreenRecorder";
#endif
		//任务参数
		using namespace devtask;
		VideoParam  videoTapingParam;
		videoTapingParam.videoDirectorPath = videoFilePath;
		videoTapingParam.videoExePath = videoExePath;
		//创建任务
		TaskParamPtr taskParam = std::make_shared<boost::any>(videoTapingParam);
		_videoTask = CPTaskController::get()->createTask(L"task.screenvideo", taskParam, false);
		CPRecord* record = _videoTask->rootRecord();
		if (!s_videoRecord) {
			s_videoRecord = record->getType()->newRecord();
			CPCase::get()->rootRecord()->addChild(s_videoRecord);
		}
		record->setFolder(false);
		s_videoRecord->addChild(record);

		_videoTask->addEndCallback([this](CPTask* task){
			_videoMovie->stop();
			_videoButton->setIcon(QPixmap()); //恢复静态图标
			_videoRunning = false;
		});
		CPTaskController::get()->doTask(_videoTask);
	}
	else {
		//关闭摄像
		_videoTask->statusCtrl()->cancel();
	}
}

void CPToolsHandler::slotCmdClicked()
{
	std::wstring cmdToolPath = siutil::CPDirHelper::get()->getToolDir() + L"/cmd/cmd.exe";
	cmdToolPath = L"\"" + cmdToolPath + L"\"";
	QProcess p(this);
	p.startDetached(QString::fromStdWString(cmdToolPath));
	p.close();
}