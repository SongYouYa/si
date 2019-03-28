#include "stdafx.h"
#include "PFilterExpDialog.h"
#include "base/PRecordTypeFactory.h"
#include "PFieldNameModel.h"
#include "POperatorModel.h"
#include "PExpWidget.h"
#include "PValue.h"

namespace filter
{
	CPListWidget::CPListWidget(const QString title, QWidget* parent /*= 0 */)
		:QWidget(parent)
	{
		QLabel *m_Labeltitle = new QLabel(title, this);
		m_Labeltitle->setObjectName("title_label");
		QHBoxLayout *labelLayout = new QHBoxLayout;
		labelLayout->setMargin(1);
		labelLayout->addSpacing(0);
		labelLayout->addWidget(m_Labeltitle);
		labelLayout->addSpacing(1);
		m_List = new QListView();
		QVBoxLayout* layout = new QVBoxLayout(this);
		layout->addLayout(labelLayout);
		layout->addWidget(m_List);
		layout->setSpacing(2);
		layout->setContentsMargins(0, 0, 7, 0);
		setLayout(layout);
	}

	void CPListWidget::setModel(QAbstractListModel* model)
	{
		m_Model = model;
		m_List->setModel(model);
	}

	CPListWidget::~CPListWidget()
	{
	}

	QAbstractListModel* CPListWidget::Model()
	{
		return m_Model;
	}

	QListView* CPListWidget::ListWidget()
	{
		return m_List;
	}

    CPFilterExpDialog::CPFilterExpDialog(QWidget* parent, const  std::wstring &fileNameType,
                                         const std::wstring &quickTypeValue)
		:CPDialog2(parent)
	{
		setWindowTitle(tr("filter_expression"));
		setFixedSize(550, 380);	
		setCancelButtonText(tr("clear"));

		QWidget* content = new QWidget();
		m_FieldNameList = new CPListWidget(tr("field_name"), this);
		CPFieldNameModel *pfileNameModel = new CPFieldNameModel(fileNameType);
		m_FieldNameList->setModel(pfileNameModel);
		m_FieldNameList->setFixedWidth(100);
		m_OperList = new CPListWidget(tr("Operator"), this);
		m_operatorMidel = new CPOperatorModel();
		m_OperList->setModel(m_operatorMidel);
		m_OperList->setFixedWidth(100);
		m_ValueWidget = new CPExpWidget(this, quickTypeValue);
		QHBoxLayout* layoutBody = new QHBoxLayout();
		layoutBody->setMargin(0);
		layoutBody->addWidget(m_FieldNameList);
		layoutBody->addWidget(m_OperList);
		layoutBody->addWidget(m_ValueWidget);
		content->setLayout(layoutBody);

		QWidget* container = new QWidget(this);
		container->setObjectName("filter_expression");
		QVBoxLayout* layout = new QVBoxLayout();
		layout->setContentsMargins(10, 5, 10, 10);
		layout->addWidget(content);
		container->setLayout(layout);
		this->setContentWidget(container);;
		setupButtons();

		connect(m_FieldNameList->ListWidget(), SIGNAL(clicked(const QModelIndex&)),
			this, SLOT(slotSelectedField(const QModelIndex&)));
		connect(m_OperList->ListWidget(), SIGNAL(clicked(const QModelIndex&)),
			this, SLOT(slotSelectedOper(const QModelIndex&)));
		//选择
		int defaultSelectIterm;
		CPRecordType *recordType = CPRecordTypeFactory::get()->getType(fileNameType);
		const CPRecordField *recordField = recordType->getDefaultFilterField();
		if (!recordField){
			defaultSelectIterm = 0; //如果找不到就设置默认的第一行为参考
		}
		else{
			defaultSelectIterm = recordType->getDefaultFilterField()->index();
		}
		QModelIndex defaultIndex = m_FieldNameList->Model()->index(defaultSelectIterm);
		m_FieldNameList->ListWidget()->setCurrentIndex(defaultIndex);
		slotSelectedField(defaultIndex);
	}

	void CPFilterExpDialog::slotSelectedOper(const QModelIndex& index)
	{
		QString oper = index.data(CPOperatorModel::Roles::OperRoleEn).toString();
		m_ValueWidget->OnOperatorChanged(oper);
	}

	void CPFilterExpDialog::slotSelectedField(const QModelIndex& index)
	{
		int type = index.data(CPFieldNameModel::FieldTypeRole).toInt();
		CPOperatorModel* model = (CPOperatorModel*)m_OperList->Model();
		model->setFieldType(CPVariant::VarType(type));
		QString fieldName = index.data(CPFieldNameModel::FieldNameRole).toString();
		fieldName = fieldName.trimmed();
		m_ValueWidget->OnValueTypeChanged(
			CPVariant::VarType(type),
			fieldName);
		//model 成默认是第一个函数
		//通过一个参数就能确定默认选择第几行
		QModelIndex defaultIndex = model->index(model->getDefaultSelectItermNum());
		m_OperList->ListWidget()->setCurrentIndex(defaultIndex);
		slotSelectedOper(defaultIndex);
	}

	void CPFilterExpDialog::onOkClicked()
	{
		//退出整个程
		m_ValueWidget->getTextResult();
		if (m_ValueWidget->getLineEditStatus()){
			CPMessageBox::warning(tr("LineEdit value not allow empty!"));
			return;
		}
		CPDialog2::onOkClicked();
	}

	void CPFilterExpDialog::onCancelClicked()
	{
		m_ValueWidget->slotClearRes();
	}

	QString CPFilterExpDialog::getReslutFilterValue()
	{
		return m_ValueWidget->getReslutValue();
	}

	void CPFilterExpDialog::autoFillInfo()
	{
		if (!m_ValueWidget->getIsPressButton()){
			m_ValueWidget->slotAddxpression();
		}
	}
}
