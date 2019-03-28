#pragma once
#include "./PCaseInfoParser.h"
#include "ui/common/widget/PDirSelectWidget.h"

class CPCaseInfoDialog : public CPDialog2
{
	Q_OBJECT
public:
	//由于案件类型数据较多，创建需要时间，所以这里使用单例，以节省时间
	static CPCaseInfoDialog* get(bool isNew, QWidget *parent = nullptr);

protected slots :
	void onOkClicked() override;
	void onThirdClicked() override; //打开按钮
	void showEvent(QShowEvent* e) override;

private:
	//isInit: 是否是第一次打开或创建案件
	CPCaseInfoDialog(bool isNew, QWidget *parent = 0);
	void init();
	void initCaseInfo(CPCaseInfoParser::PCaseInfo& caseInfo);
	bool checkValid();
	bool saveCaseInfo();

	QLineEdit *_lineEdit_num; //案件编号
	QLineEdit *_lineEdit_name; //案件名称
	QComboBox* _combo_type;
	QLineEdit *_lineEdit_investigator;//调查人
	QLineEdit *_lineEdit_witness;
	QDateTimeEdit *_lineEdit_real_time;
	QLineEdit *_lineEdit_address;
	QLineEdit * _lineEdit_suspect;
	CPDirSelectWidget* _dir_selecter;
	QTextEdit *_textEdit_remark;

	std::map<std::string, std::wstring> _itemValues;

	bool _isNew; //是否新建案件（打开案件）

	static CPCaseInfoDialog* _instance;
};
