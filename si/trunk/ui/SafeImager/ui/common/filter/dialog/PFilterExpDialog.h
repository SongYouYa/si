#pragma once

namespace filter
{
	class CPExpWidget;
	class CPOperatorModel;
	class CPListWidget : public QWidget
	{
		Q_OBJECT
	public:
		CPListWidget(const QString title, QWidget* parent /*= 0 */);
		~CPListWidget();
		void setModel(QAbstractListModel* model);
		QAbstractListModel* Model();
		QListView* ListWidget();
	private:
		QListView* m_List;
		QAbstractListModel* m_Model;
	};

	class CPFilterExpDialog : public CPDialog2
	{
		Q_OBJECT
	public:
        CPFilterExpDialog(QWidget* parent = nullptr, const std::wstring &fileNameType = L"",
                          const std::wstring &quickTypeValue = L"");
		QString ExpDisplay();
		QString getReslutFilterValue();
		void autoFillInfo();

	private slots:
		void slotSelectedField(const QModelIndex& index);
		void slotSelectedOper(const QModelIndex& index);

	private:
		void onOkClicked() override;
		void onCancelClicked() override;

		CPListWidget* m_FieldNameList;
		CPOperatorModel * m_operatorMidel;
		CPListWidget* m_OperList;
		CPExpWidget* m_ValueWidget;
		std::wstring _quickTypeValue;
		CPVariant::VarType _operatorType;
	};

}
