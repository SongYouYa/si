#pragma once
#include "uicommon/filesystem/PFsTreelistWidget.h"
#include "ui/common/progress/PLineLoadingWidget.h"

class CPExportOptionDialog;
class CPFilesystemView : public QWidget
{
	Q_OBJECT
public:
	CPFilesystemView(QWidget* parent = nullptr);

	private slots:
	void slotLoadFinished(bool suc);
	void slotExportClicked();
	void slotChangeZipBoxState();
	void slotChangeFileBoxState();

signals:
	void signalStateChanged(int state);

private:
	void showEvent(QShowEvent* e) override;

	CPFsTreelistWidget* _treelistWidget;
	CPExportOptionDialog* _exportOptionDialog;

	CPLineLoadingWidget* _lineLoading;
	QCheckBox* _exportFileBox;
	QCheckBox* _exportListBox;
	QCheckBox* _createZipBox;
	QPushButton* _startButton;
};
