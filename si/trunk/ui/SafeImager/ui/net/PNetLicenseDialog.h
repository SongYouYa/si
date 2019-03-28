#pragma once
#include "uicommon/progress/PLoadingWidget.h"

class CPNetLicenseDialog : public CPDialog2
{
	Q_OBJECT
public:
	CPNetLicenseDialog(QWidget* parent = nullptr);
	void setPort(int port);

signals:
	void signalApplyResult(int);

	private slots:
	void slotApplyReslut(int);

private:
	void onOkClicked() override;
	void closeEvent(QCloseEvent * e) override;

	QLineEdit* _licEdit;
	CPLoadingWidget* _loadingWidget;
	QLabel* _msgLabel;
};