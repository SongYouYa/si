#include "stdafx.h"
#include "PValue.h"
#include "POperatorModel.h"
#include "base/PVariant.h"
#include "PFieldNameModel.h"

namespace filter
{
	CPOperatorModel::CPOperatorModel()
	{
	}

	CPOperatorModel::~CPOperatorModel()
	{
	}

	void CPOperatorModel::setFieldType(const CPVariant::VarType type)
	{
		beginResetModel();
		m_Operators.clear();
		//通过这里可以修改展示的数据类型
		if (type == CPVariant::VarType::TDateTime)
		{
			_selectTypeNum = SELECTTYPE::T_DATA;
			m_Operators.push_back(E_RELATION_TYPE::E_EQUAL);
			m_Operators.push_back(E_RELATION_TYPE::E_NEQUAL);
			m_Operators.push_back(E_RELATION_TYPE::E_SMALLER);
			m_Operators.push_back(E_RELATION_TYPE::E_BIGGER);
			m_Operators.push_back(E_RELATION_TYPE::E_SAEQUAL);
			m_Operators.push_back(E_RELATION_TYPE::E_GAEQUAL);
			findDefaultItermNum(m_Operators);
		}
		else if (type == CPVariant::VarType::TInt64)
		{
			_selectTypeNum = SELECTTYPE::T_INT;//标记选择的类型
			m_Operators.push_back(E_RELATION_TYPE::E_EQUAL);
			m_Operators.push_back(E_RELATION_TYPE::E_NEQUAL);
			m_Operators.push_back(E_RELATION_TYPE::E_SMALLER);
			m_Operators.push_back(E_RELATION_TYPE::E_BIGGER);
			m_Operators.push_back(E_RELATION_TYPE::E_SAEQUAL);
			m_Operators.push_back(E_RELATION_TYPE::E_GAEQUAL);
			findDefaultItermNum(m_Operators);
		}
		else if (type == CPVariant::VarType::TString)
		{
			_selectTypeNum = SELECTTYPE::T_STRING;
			m_Operators.push_back(E_RELATION_TYPE::E_EQUAL);
			m_Operators.push_back(E_RELATION_TYPE::E_NEQUAL);
			m_Operators.push_back(E_RELATION_TYPE::E_CONTAIN);
			m_Operators.push_back(E_RELATION_TYPE::E_EXCLUDE);
			findDefaultItermNum(m_Operators);
		}
		endResetModel();
	}

	int CPOperatorModel::rowCount(const QModelIndex &parent /*= QModelIndex() */) const
	{
		return m_Operators.size();
	}

	QVariant CPOperatorModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole */) const
	{
		if (!index.isValid())
			return QVariant();
		size_t row = (size_t)index.row();
        if (role == OperRoleType && row < (size_t)rowCount())
		{
			return m_Operators.at(row);
		}
		if (role == Qt::DisplayRole)
		{
			return QString(" ") + Display(m_Operators.at(row));
		}
		if (role == OperRoleEn)
		{
			return Display(m_Operators.at(row), role);
		}
		return QVariant();
	}

	QString CPOperatorModel::Display(E_RELATION_TYPE oper, int role /*= Qt::DisplayRole*/)
	{
		//在这里也相当于一个容器
		static tagRelation arrDisplay[E_RELATION_TYPE::E_RELATION_INVALID] = {
			{ L"等于", L"==", E_EQUAL },
			{ L"不等于", L"!=", E_NEQUAL },
			{ L"小于", L"<", E_SMALLER },
			{ L"大于", L">", E_BIGGER },
			{ L"小于等于", L"<=", E_SAEQUAL },
			{ L"大于等于", L">=", E_GAEQUAL },
			{ L"包含", L"inc", E_CONTAIN },
			{ L"不包含", L"exc", E_EXCLUDE },
			{ L"包含在", L"in", E_IN }
		};
		if (oper < E_RELATION_INVALID && oper >= 0)
		{
			QString ch = QString::fromStdWString(arrDisplay[oper].chChs);
			QString en = QString::fromStdWString(arrDisplay[oper].chEn);
			if (role == Qt::DisplayRole)
			{
				return QString("%1(%2)").arg(ch, en);
			}
			if (role == Roles::OperRoleEn)
			{
				return en;
			}
		}
		return "";
	}
	std::vector<E_RELATION_TYPE> CPOperatorModel::getVectorOperator()
	{
		return m_Operators;
	}
	void CPOperatorModel::findDefaultItermNum(const std::vector<E_RELATION_TYPE> &type)
	{
		_operDefaultSelectType = 0;
		if (_selectTypeNum == SELECTTYPE::T_DATA){ //根据不同的类型进行选择
			for (auto &operType : type){
				if (operType == E_RELATION_TYPE::E_SAEQUAL){ //遍历vector确定要选择的类型
					break;
				}
				_operDefaultSelectType++;
			}
		}
		else if (_selectTypeNum == SELECTTYPE::T_INT){
			for (auto &operType : type){
				if (operType == E_RELATION_TYPE::E_EQUAL){
					break;
				}
				_operDefaultSelectType++;
			}
		}
		else if (_selectTypeNum == SELECTTYPE::T_STRING){
			for (auto &operType : type){
				if (operType == E_RELATION_TYPE::E_CONTAIN){
					break;
				}
				_operDefaultSelectType++;
			}
		}
	}
}
