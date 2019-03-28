#pragma once
#include "PFilterDef.h"
#include "base/PVariant.h"
#include "./PValue.h"

namespace filter
{
	class CPLineEdit;
	class CPToolButton;
	class CPExpWidget : public QWidget
	{
		Q_OBJECT
			// ENABLE_CSS_FOR_QWIDGET_SUBCLASSED
	public:
        CPExpWidget(QWidget* parent = nullptr, const std::wstring &quicTypeValue = L"");
		void OnValueTypeChanged(
			CPVariant::VarType type,       //类型
			const QString& fieldname);    //枚举类型，非枚举类型，该参数为空 
		QString getReslutValue();
		bool getLineEditStatus();
		QTextEdit *getTextResult();
		bool getIsPressButton(){
			return _isPressedTag;
		};
		void OnOperatorChanged(const QString& strOper);

	public slots:
		void slotAddxpression();
		void slotClearRes();

	private:
		void initValueWidget();
		QStringList enumValueString(const QString& name);
		QString    enumValue(int index);
		QString CurrentExp(bool bDisplay = true);
	private slots:
		void slotStringChanged(const QString& value);
		void slotDateTimeChanged(const QDateTime& date);
		void slotEditChanger();

	private:
		ExpValueString  _expressValue;
		QString m_ExpLastValue;  //最后表达式的值
		CPVariant::VarType m_DataType;
		QString m_strOper;  // operator : < > !=
		QString m_join;     // and / or
		int     m_expCount;
		bool isShowLineEdiWarring = false;
		bool _isPressedTag = false;
		QLabel*     m_LabelName;
		QLineEdit*  m_pEditValue;
		QLabel*		 m_pLabelExp;
		QDateTimeEdit* m_ValueDateTime;
		QRadioButton* m_btnAnd;
		QRadioButton* m_btnOr;
		QPushButton*  m_pBtnPlus;
		QTextEdit*	m_pEditResult;
		QWidget* m_ResWidget;
		QWidget* m_AddWidget;
		QWidget* m_valueWidget;
		QWidget* m_ExpWidget;
		QPushButton*  m_btnAdd;
		QLabel* m_labelExp;
		QString m_exp;
		QLabel* m_labelName;
		CPVariant::VarType _type;
	};
}
