#pragma once


class CPToolsHandler: public QObject
{
	Q_OBJECT
public:
	CPToolsHandler(QObject* parent = nullptr);

	//截屏时，隐藏的对象
	void setMainWindow(QWidget* widget) {
		_mainWindow = widget;
	}

	void setCaseButton(QToolButton* button);
	void setCmdButton(QToolButton* button);
	void setPhotoButton(QToolButton* button);
	void setVideoButton(QToolButton* button);

protected slots:
	void slotCaseClicked();
	void slotPhotoClicked();
	void slotOnScreenTimeout();
	void slotVideoClicked();
	void slotCmdClicked();

private:
	QTimer *_photoTimer;
	QToolButton* _videoButton;
	QWidget* _mainWindow;

	bool _videoRunning;
	QMovie* _videoMovie;
	CPTask* _videoTask;
};