#include "stdafx.h"
#include "ui/content/case/PCaseInfoDialog.h"
#include "uicommon/widget/PTextEdit.h"
#include "logic/case/PCaseTypeHandler.h"
#include "logic/case/PCaseCatagoryHandler.h"

#include "siio/PNormalFile.h"
#include "sicommon/PString.h"
#include "logic/PReportStatus.h"


QString PTIME_FORMAT = "yyyy-MM-dd HH:mm:ss";
QRegExp VALID_FILENAME_REG("([^?:*|/<>\\\\]*)"); //限制非法字符

extern CPRecord* g_caseInfoFolder;

CPCaseInfoDialog* CPCaseInfoDialog::_instance = nullptr;

CPCaseInfoDialog::CPCaseInfoDialog(bool isNew, QWidget *parent)
: CPDialog2(parent),
_isNew(isNew)
{
	QWidget* container = new QWidget(this);
	container->setObjectName("content_container");
	setWindowTitle(tr("case_infomation"));
	resize(380, height());

	QLabel *lable_number = new QLabel(tr("case_number"));
	QLabel *lable_name = new QLabel(tr("case_name"));
	QLabel *lable_type = new QLabel(tr("case_type"));
	QLabel *lable_investigator = new QLabel(tr("investigator"));
	QLabel *lable_witness = new QLabel(tr("witness"));
	QLabel *lable_actual_time = new QLabel(tr("case_time"));
	QLabel *lable_address = new QLabel(tr("case_address"));
	QLabel *lable_suspect = new QLabel(tr("case_suspect"));
	QLabel* lable_casedir = new QLabel(tr("case_dir"));
	QLabel *label_remark = new QLabel(tr("remark"));
	
	QRegExpValidator *pReg = new QRegExpValidator(VALID_FILENAME_REG, this);
	_lineEdit_num = new QLineEdit;
	_lineEdit_name = new QLineEdit;
	_lineEdit_name->setValidator(pReg);
	_combo_type = new QComboBox();
	_combo_type->setEditable(true);
	_lineEdit_investigator = new QLineEdit;
	_lineEdit_witness = new QLineEdit;
	_lineEdit_real_time = new QDateTimeEdit;
	_lineEdit_real_time->setDisplayFormat(PTIME_FORMAT);
	_lineEdit_real_time->setButtonSymbols(QAbstractSpinBox::NoButtons);
	_lineEdit_address = new QLineEdit;
	_lineEdit_suspect = new QLineEdit;
	_dir_selecter = new CPDirSelectWidget(this);
	_textEdit_remark = new CPTextEdit;
	_textEdit_remark->setFixedHeight(45);
	
	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->setMargin(15);
	gridLayout->setHorizontalSpacing(15);

	gridLayout->addWidget(lable_name, 0, 0);
	gridLayout->addWidget(_lineEdit_name, 0, 1);
	gridLayout->addWidget(lable_number, 1, 0);
	gridLayout->addWidget(_lineEdit_num, 1, 1);
	gridLayout->addWidget(lable_type, 2, 0);
	gridLayout->addWidget(_combo_type, 2, 1);
	gridLayout->addWidget(lable_actual_time, 3, 0);
	gridLayout->addWidget(_lineEdit_real_time, 3, 1);
	gridLayout->addWidget(lable_investigator, 4, 0);
	gridLayout->addWidget(_lineEdit_investigator, 4, 1);
	gridLayout->addWidget(lable_witness, 5, 0);
	gridLayout->addWidget(_lineEdit_witness, 5, 1);
	
	gridLayout->addWidget(lable_address, 6, 0);
	gridLayout->addWidget(_lineEdit_address, 6, 1);
	gridLayout->addWidget(lable_suspect, 7, 0);
	gridLayout->addWidget(_lineEdit_suspect, 7, 1);
	gridLayout->addWidget(lable_casedir, 8, 0);
	gridLayout->addWidget(_dir_selecter, 8, 1);
	gridLayout->addWidget(label_remark, 9, 0);
	gridLayout->addWidget(_textEdit_remark, 9, 1); //fromRow：行号；fromColumn：列号；rowSpan：占多少行；columnSpan：占多少列
												
	container->setLayout(gridLayout);			//主布局装到对话框
	this->setContentWidget(container);			//放wiget
	setupButtons();
	
	setThirdButtonText(tr("case dir"));

	//case type
	auto typeModel = CPCaseTypeHandler::get()->getDetailModel();
	_combo_type->setModel(typeModel);
	QCompleter *completer = new QCompleter();
	completer->setFilterMode(Qt::MatchContains);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setModel(typeModel);
	_combo_type->setCompleter(completer);
}

CPCaseInfoDialog* CPCaseInfoDialog::get(bool isNew, QWidget *parent)
{
	if (!_instance) {
		_instance = new CPCaseInfoDialog(isNew, parent);
	}
	_instance->_isNew = isNew;
	return _instance;
}

void CPCaseInfoDialog::initCaseInfo(CPCaseInfoParser::PCaseInfo& caseInfo)
{
	_lineEdit_num->setText(QString::fromStdWString(caseInfo[PCASE_NUMBER])); 
	_lineEdit_name->setText(QString::fromStdWString(caseInfo[PCASE_NAME]));
	_lineEdit_investigator->setText(QString::fromStdWString(caseInfo[PCASE_INV]));
	_lineEdit_witness->setText(QString::fromStdWString(caseInfo[PCASE_VIT]));
	QDateTime realTime = QDateTime::fromString(QString::fromStdWString(caseInfo[PCASE_RT]), PTIME_FORMAT);
	_lineEdit_real_time->setDateTime(realTime);
	_lineEdit_address->setText(QString::fromStdWString(caseInfo[PCASE_ADD]));
	_lineEdit_suspect->setText(QString::fromStdWString(caseInfo[PCASE_SUSPT]));
	_dir_selecter->setPath(QString::fromStdWString(caseInfo[PCASE_DIR]));
	_textEdit_remark->setText(QString::fromStdWString(caseInfo[PCASE_REMARK]));

	//初始化类型选项
	QString caseTypeText = QString::fromStdWString(caseInfo[PCASE_TYPE]);
	int index = _combo_type->findText(caseTypeText, Qt::MatchStartsWith);
	if (index >= 0) {
		_combo_type->setCurrentIndex(index);
	}
	else {
		_combo_type->setCurrentText(caseTypeText);
	}
}

void CPCaseInfoDialog::init()
{
	CPCaseInfoParser::PCaseInfo caseInfo;
	if (_isNew) {
		CPCaseInfoParser::getCaseInfo(caseInfo);
	}
	else {
		_lineEdit_name->setDisabled(true);
		_dir_selecter->setDisabled(true);
		std::wstring caseFile = CPCase::get()->filePath();
		CPCaseInfoParser::getCaseInfo(caseFile, caseInfo);
#if 0
		setThirdHidden(false);
#endif
	}
	initCaseInfo(caseInfo);
}

void CPCaseInfoDialog::showEvent(QShowEvent* e)
{
	init();
	CPDialog2::showEvent(e);
}

void CPCaseInfoDialog::onOkClicked()
{
	if (!saveCaseInfo()) {
		return;
	}
	CPDialog::onOkClicked();					//调用父类,点击确定，返回值
}

void CPCaseInfoDialog::onThirdClicked()
{
	std::wstring caseDir = CPCase::get()->caseDir();
	if (caseDir.empty()) {
		return;
	}
	QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdWString(caseDir)));
}

bool CPCaseInfoDialog::checkValid()
{
	QString emptyStr = tr("can not be empty!");
	QString caseName = _lineEdit_name->text().trimmed();
	if (caseName.isEmpty()) {
		CPMessageBox::warning(tr("case name") + emptyStr);
        _lineEdit_name->setFocus(Qt::ActiveWindowFocusReason);
		return false;
	}
	return true;
}

bool CPCaseInfoDialog::saveCaseInfo()
{
    if (CPCase::caseRootDir().empty()) {
        CPMessageBox::warning(tr("save dir not setted!"));
        return false;
    }
	if (!checkValid()) {
		return false;
	}
	QString caseName = _lineEdit_name->text().trimmed();
	if (!CPCase::get()->creatCase(caseName.toStdWString(), _isNew)) {
		CPMessageBox::warning(tr("create case error!"));
		return false;
	}
	std::wstring caseFilePath = CPCase::get()->filePath();

	if (_isNew) {
		QFile file(QString::fromStdWString(caseFilePath));
		if (file.exists()) {
			int ret = CPMessageBox::question(tr("The case already exists, to override it?"));
			if (CPDialog::Accepted == ret) {
				std::wstring caseDir = CPCase::get()->caseDir();
				try {
					boost::filesystem::remove_all(caseDir);
				}
				catch (...) {
					CPMessageBox::warning(tr("remove old case dir error!"));
					return false;
				}
			}
			else {
				return false;
			}	
		}
	}

	CPCase::get()->filePath();	
	CPCaseInfoParser::PCaseInfo caseInfo;
	caseInfo[PCASE_NUMBER] = _lineEdit_num->text().toStdWString();
	caseInfo[PCASE_NAME] = _lineEdit_name->text().toStdWString();
	QString caseTypeText = _combo_type->currentText();
	caseInfo[PCASE_TYPE] = caseTypeText.toStdWString();
	caseInfo[PCASE_INV] = _lineEdit_investigator->text().toStdWString();
	caseInfo[PCASE_VIT] = _lineEdit_witness->text().toStdWString();
	caseInfo[PCASE_RT] = _lineEdit_real_time->text().toStdWString();
	caseInfo[PCASE_ADD] = _lineEdit_address->text().toStdWString();
	caseInfo[PCASE_SUSPT] = _lineEdit_suspect->text().toStdWString();
	caseInfo[PCASE_DIR] = _dir_selecter->getPath().toStdWString();
	caseInfo[PCASE_REMARK] = _textEdit_remark->toPlainText().toStdWString();
	bool ret = CPCaseInfoParser::saveCaseInfo(caseInfo, caseFilePath, _isNew);

	CPRecord* infoRecord = CPCase::get()->infoRecord();
	if (!infoRecord) {
		infoRecord = g_caseInfoFolder->addChild();
		CPCase::get()->setInfoRecord(infoRecord);
	}
	CPCaseInfoParser::caseInfo2Record(caseInfo, infoRecord);
	CPCase::get()->update();

	QString caseCatagory = CPCaseTypeHandler::get()->getCatagory(caseTypeText);
	CPCaseCatagoryHandler::get()->setCatagory(caseCatagory);

	return ret;
}
