#pragma once

namespace filter
{
	class  CPFieldNameModel : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum Roles
		{
			FieldTypeRole = Qt::UserRole + 1,
			FieldNameRole
		};
		CPFieldNameModel(const std::wstring &nameValueType);
		~CPFieldNameModel();
		std::vector<std::wstring> getNameType(){
			return _name_types_vector;
		}
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	private:
		std::wstring  m_nameValueType;
		std::vector<std::wstring> _display_types_vector;
		std::vector<std::wstring> _name_types_vector;
		std::vector<CPVariant::VarType> _nameType;

	};
}