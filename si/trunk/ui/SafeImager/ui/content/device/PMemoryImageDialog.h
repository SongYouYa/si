#pragma once
#include "./PMemoryProgressDialog.h"

class CPMemoryImageWidget;
class CPMemoryImageDialog : public CPDialog2
{
	Q_OBJECT
public:
	CPMemoryImageDialog(QWidget* parent = nullptr);

	private slots:
	void onImageStart();

protected:
	void onOkClicked() override;
	void closeEvent(QCloseEvent* e) override;

	CPMemoryImageWidget* _imageWidget;
	CPMemoryProgressDialog _progressDialog;
};