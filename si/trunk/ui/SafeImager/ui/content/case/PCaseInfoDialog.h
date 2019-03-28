#pragma once
#include "./PCaseInfoParser.h"
#include "ui/common/widget/PDirSelectWidget.h"

class CPCaseInfoDialog : public CPDialog2
{
	Q_OBJECT
public:
	//���ڰ����������ݽ϶࣬������Ҫʱ�䣬��������ʹ�õ������Խ�ʡʱ��
	static CPCaseInfoDialog* get(bool isNew, QWidget *parent = nullptr);

protected slots :
	void onOkClicked() override;
	void onThirdClicked() override; //�򿪰�ť
	void showEvent(QShowEvent* e) override;

private:
	//isInit: �Ƿ��ǵ�һ�δ򿪻򴴽�����
	CPCaseInfoDialog(bool isNew, QWidget *parent = 0);
	void init();
	void initCaseInfo(CPCaseInfoParser::PCaseInfo& caseInfo);
	bool checkValid();
	bool saveCaseInfo();

	QLineEdit *_lineEdit_num; //�������
	QLineEdit *_lineEdit_name; //��������
	QComboBox* _combo_type;
	QLineEdit *_lineEdit_investigator;//������
	QLineEdit *_lineEdit_witness;
	QDateTimeEdit *_lineEdit_real_time;
	QLineEdit *_lineEdit_address;
	QLineEdit * _lineEdit_suspect;
	CPDirSelectWidget* _dir_selecter;
	QTextEdit *_textEdit_remark;

	std::map<std::string, std::wstring> _itemValues;

	bool _isNew; //�Ƿ��½��������򿪰�����

	static CPCaseInfoDialog* _instance;
};
