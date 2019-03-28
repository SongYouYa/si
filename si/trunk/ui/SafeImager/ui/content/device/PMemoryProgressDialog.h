#pragma once
#include "uicommon/task/PTaskStatusHandler.h"
#include "uicommon/task/PTaskProgressHandler.h"
#include "uicommon/task/PTaskCallbackHandler.h"
#include "ui/common/widget/PDirSelectWidget.h"

class CPMemoryProgressDialog : public CPDialog2
{
	Q_OBJECT
public:
	CPMemoryProgressDialog(QWidget* parent = nullptr);

	void setTask(CPTask* task);
	void setImagePath(CPFilePathContainer* pathContainer);

protected:
	void closeEvent(QCloseEvent* e) override;

private:
	CPTaskStatusHandler* _statusHandler;
	CPTaskProgressHandler* _progressHandler;
	CPTaskCallbackHandler* _callbackHandler;

	CPTask* _task;
	CPDirSelectWidget* _imageDirWidget;
	CPFileNameEdit* _filenameEdit;
};
