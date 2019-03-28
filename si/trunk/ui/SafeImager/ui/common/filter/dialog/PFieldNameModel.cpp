#include "stdafx.h"
#include "base/PRecordTypeFactory.h"
#include "base/PRecordType.h"
#include "PFieldNameModel.h"

namespace filter
{
	CPFieldNameModel::CPFieldNameModel(const std::wstring &nameValueType)
		:m_nameValueType(nameValueType)
	{
		CPRecordType *recordType = CPRecordTypeFactory::get()->getType(nameValueType);
		RecordFieldList recordFieldlists = recordType->getPublicFields();
		for (auto & recordType : recordFieldlists){
			_display_types_vector.push_back(recordType->display());
			_name_types_vector.push_back(recordType->name());
			_nameType.push_back(recordType->type());
		}
	}

	CPFieldNameModel::~CPFieldNameModel()
	{

	}

	int CPFieldNameModel::rowCount(const QModelIndex &parent /*= QModelIndex() */) const
	{

		return _name_types_vector.size();
	}

	QVariant CPFieldNameModel::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();

		if (role == Qt::DisplayRole){
			return QString(" ") + QString::fromStdWString(_display_types_vector.at(index.row()));
		}
		if (role == FieldNameRole)
		{
			return QString(" ") + QString::fromStdWString(_name_types_vector.at(index.row()));
		}
		if (role == FieldTypeRole)
		{
			CPVariant::VarType type = _nameType.at(index.row());
			if (type == CPVariant::VarType::TString)
			{
				return CPVariant::VarType::TString;
			}
			if (type == CPVariant::VarType::TDateTime)
			{
				return CPVariant::VarType::TDateTime;
			}
			if (type == CPVariant::VarType::TInt64)
			{
				return CPVariant::VarType::TInt64;
			}
			else{

			}
		}
		return QVariant();
	}
}