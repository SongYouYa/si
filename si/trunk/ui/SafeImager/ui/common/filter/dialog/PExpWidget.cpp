#include "stdafx.h"
#include "PValue.h"
#include "PExpWidget.h"
#include "uicommon/button/PToolButton.h"


namespace filter
{
    CPExpWidget::CPExpWidget(QWidget* parent, const std::wstring &quicTypeValue)
		:QWidget(parent)
		, m_expCount(0), _expressValue({ "", "" })
	{
		m_pEditValue = new QLineEdit; //for string or int
		m_pEditValue->setMinimumHeight(27);
		m_ValueDateTime = new QDateTimeEdit;
		m_ValueDateTime->setButtonSymbols(QAbstractSpinBox::NoButtons);
		m_ValueDateTime->setMinimumHeight(27);
		m_ValueDateTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
		m_ValueDateTime->setDateTime(QDateTime::currentDateTime());

		m_valueWidget = new QWidget(this);
		m_labelName = new QLabel(m_valueWidget);//值
		m_labelName->setMaximumWidth(13);
		m_labelName->setText(tr("value"));
		m_labelName->setAlignment(Qt::AlignCenter);

		//添加头layout
		QHBoxLayout* layoutValue = new QHBoxLayout;
		layoutValue->setMargin(0);
		layoutValue->addWidget(m_labelName);
		layoutValue->addSpacing(4);
		layoutValue->addWidget(m_pEditValue);
		layoutValue->addWidget(m_ValueDateTime);
		m_valueWidget->setLayout(layoutValue);
		//表达式widget
		m_ExpWidget = new QWidget();
		m_labelExp = new QLabel(m_ExpWidget);
		m_labelExp->setText(tr("expression:"));
		QHBoxLayout* layoutExp = new QHBoxLayout;
		layoutExp->setMargin(0);
		layoutExp->addWidget(m_labelExp);
		m_pLabelExp = new QLabel();
		m_pLabelExp->setAlignment(Qt::AlignLeft);

		layoutExp->addWidget(m_pLabelExp);
		layoutExp->setAlignment(Qt::AlignLeft);
		m_ExpWidget->setLayout(layoutExp);
		//添加按钮
		m_AddWidget = new QWidget(this);
		QHBoxLayout* layoutAdd = new QHBoxLayout;
		layoutAdd->setMargin(0);
		m_btnAnd = new QRadioButton(m_AddWidget);
		m_btnAnd->setText(tr("And"));
		layoutAdd->addWidget(m_btnAnd);
		layoutAdd->addSpacing(30);
		m_btnOr = new QRadioButton(m_AddWidget);
		m_btnOr->setText(tr("Or"));
		layoutAdd->addWidget(m_btnOr);

		layoutAdd->addStretch();
		m_btnAdd = new QPushButton(m_AddWidget);
		m_btnAdd->setObjectName("add_expr");
		m_btnAdd->setFixedSize(17, 17);
		
		layoutAdd->addWidget(m_btnAdd);
		m_btnAnd->setChecked(true);
		m_AddWidget->setLayout(layoutAdd);

		//表达式结果
		m_ResWidget = new QWidget(this);
		QHBoxLayout* layoutRes = new QHBoxLayout;
		m_pEditResult = new QTextEdit(m_ResWidget);
		m_pEditResult->setText(QString::fromStdWString(quicTypeValue));
		layoutRes->addWidget(m_pEditResult);

		QVBoxLayout* mianLayout = new QVBoxLayout();
		mianLayout->setSpacing(0);
		mianLayout->setContentsMargins(0, 22, 0, 0);
		mianLayout->addWidget(m_valueWidget);
		mianLayout->addSpacing(7);
		mianLayout->addWidget(m_ExpWidget);
		mianLayout->addSpacing(7);
		mianLayout->addWidget(m_AddWidget);
		mianLayout->addSpacing(7);
		mianLayout->addLayout(layoutRes);
		                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
		setLayout(mianLayout);
		//QTextEdit::textChanged();
		OnValueTypeChanged(CPVariant::VarType::TDateTime, QString());

		connect(m_pEditResult, SIGNAL(textChanged()), this, SLOT(slotEditChanger()));
		connect(m_pEditValue, SIGNAL(textChanged(const QString&)),
			this, SLOT(slotStringChanged(const QString&)));
		connect(m_ValueDateTime, SIGNAL(dateTimeChanged(const QDateTime&)),
			this, SLOT(slotDateTimeChanged(const QDateTime&)));
		connect(m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(slotAddxpression()));
	}

	void CPExpWidget::initValueWidget()
	{
		if (m_DataType == CPVariant::VarType::TDateTime)
		{
			m_ValueDateTime->show();
			m_ValueDateTime->setDateTime(QDateTime::currentDateTime());
		}
		else if (m_DataType == CPVariant::VarType::TInt64)
		{
			m_pEditValue->show();
			QRegExp rx("[0-9]+");
			m_pEditValue->setValidator(new QRegExpValidator(rx, this));
		}
		else if (m_DataType == CPVariant::VarType::TString)
		{
			m_pEditValue->show();
			QRegExp rx("*");
			m_pEditValue->setValidator(0);
		}
		else {  //handle 类型
			m_pEditValue->show();
			QRegExp rx("*");
			m_pEditValue->setValidator(0);
		}
	}
	void CPExpWidget::OnValueTypeChanged(
		CPVariant::VarType type,       //类型
		const QString& fieldname   //字段名
		)
	{
		m_pEditValue->hide();
		m_ValueDateTime->hide();
		m_pEditValue->clear();
		m_DataType = type;
		_type = type;
		_expressValue.value = fieldname;
		_expressValue.strType = fieldname;
		initValueWidget();
		m_pLabelExp->setText(CurrentExp());
	}

	void CPExpWidget::OnOperatorChanged(const QString& strOper)
	{
		m_strOper = strOper;
		QString  nameString = _expressValue.value + " " + m_strOper;
		m_pLabelExp->setText(nameString);
		if (_type == CPVariant::TDateTime){
			slotDateTimeChanged(m_ValueDateTime->dateTime());
		}
	}

	QString CPExpWidget::getReslutValue()
	{
		return m_pEditResult->toPlainText();
	}

	void CPExpWidget::slotStringChanged(const QString& value)
	{
		_expressValue.value = value;
		m_pLabelExp->setText(CurrentExp());
	}
	void CPExpWidget::slotDateTimeChanged(const QDateTime& date)
	{
		_expressValue.value = date.toString("yyyy-MM-dd hh:mm:ss");
		QString data = CurrentExp();
		m_pLabelExp->setText(data);
	}
	bool CPExpWidget::getLineEditStatus()
	{
		return  isShowLineEdiWarring;

	}
	void CPExpWidget::slotAddxpression()
	{
		//判断是否发生改变，没有说明没有变化
		if (_type != CPVariant::TDateTime){
			if (m_pEditValue->text().isEmpty()){
				return;
			}
		}
		m_join = m_btnAnd->isChecked() ? "And" : "Or";
		if (_expressValue.value.isEmpty()){
			return;
		}
		QString display = CurrentExp(true); //翻译后的表达式
		//第一次不加and 或者or符号
		QString editText = m_pEditResult->toPlainText();
		if (editText.isEmpty()){
			m_join = "";
		}
		display = editText + " " + m_join + " " + display + " ";
		m_pEditResult->setText(display);
		m_ExpLastValue = display; //记录最后的edit数据
		//点击添加按钮过后m_pEditValue显示对话框清零
		m_pEditValue->clear();
		_isPressedTag = true;
	}

	void CPExpWidget::slotEditChanger()
	{
		m_ExpLastValue = m_pEditResult->toPlainText();
	}

	QString CPExpWidget::CurrentExp(bool bDisplay /*= true*/)
	{
		QString dataText = _expressValue.value;
		QString strName = _expressValue.strType;

		QString  nameString = "";
		if (m_DataType == CPVariant::VarType::TInt64){
			nameString = strName + " " + m_strOper + " " + dataText;
		}
		else{
			nameString = strName + " " + m_strOper + " " + "\"" + dataText + "\"";
		}
		return nameString;

	}

	QTextEdit * CPExpWidget::getTextResult()
	{
		//都为空的情况下设置且
		bool visible = m_pEditValue->isVisible();
		if ((m_pEditResult->toPlainText().isEmpty()) && (m_pEditValue->text().isEmpty() && (m_pEditValue->isVisible()))){
			isShowLineEdiWarring = true;
		}
		else{
			isShowLineEdiWarring = false;
		}
		return  m_pEditResult;
	}

	void CPExpWidget::slotClearRes()
	{
		m_ExpLastValue = "";
		m_pEditResult->clear();
		m_expCount = 0;
	}

}
