#pragma once
#include "uicommon/filter/PFilterBarBase.h"
#include "ui/common/edit/PSearchEdit.h"

class CPFilterBarWidget : public CPFilterBarBase
{
	Q_OBJECT
public:
	CPFilterBarWidget(QWidget* parent = nullptr);

	void setType(const std::wstring& filterType) override;

	//����APP����ǰ��ִ�лص������ڽ��ý���ؼ�
	void setPreCallback(const VoidCallback& callback) {
		_preCallback = callback;
	}

	protected slots:
	void slotOpenDialog();
	void slotCancelClicked();
	void slotEnterPressed();

protected:
	virtual void initLeftLayout() {};
	virtual void startFilte(bool force = false);

	QHBoxLayout* _leftLayout;
	CPSearchEdit* _searchEdit;
	QPushButton* _addButton;

	VoidCallback _preCallback;
};