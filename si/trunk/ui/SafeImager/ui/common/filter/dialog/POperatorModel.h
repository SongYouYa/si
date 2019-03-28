#pragma once
#include "PFilterDef.h"

namespace filter
{
	class  CPOperatorModel : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum Roles
		{
			OperRoleType = Qt::UserRole + 1,
			OperRoleEn
		};
		enum SELECTTYPE{ T_DATA=0, T_STRING=1, T_INT=2 };
		
		CPOperatorModel();
		~CPOperatorModel();
		std::vector<E_RELATION_TYPE> getVectorOperator();
		void setFieldType(const CPVariant::VarType type);
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		int getDefaultSelectItermNum(){
			return _operDefaultSelectType;
		}

	private:
		int _selectTypeNum = 0;
		int _operDefaultSelectType = 0;
		static QString Display(E_RELATION_TYPE oper, int role = Qt::DisplayRole);
		std::vector<E_RELATION_TYPE> m_Operators;
        void findDefaultItermNum(const std::vector<E_RELATION_TYPE> &type);
	};
}
