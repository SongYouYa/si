#include "stdafx.h"
#include "PCaseTypeHandler.h"


CPCaseTypeHandler* CPCaseTypeHandler::_instance = nullptr;

CPCaseTypeHandler* CPCaseTypeHandler::get()
{
	if (!_instance) {
		_instance = new CPCaseTypeHandler();
	}
	return _instance;
}

CPCaseTypeHandler::CPCaseTypeHandler()
	:_detailModel(nullptr), _catagoryModel(nullptr)
{
	
}

bool CPCaseTypeHandler::loadTxt(const std::wstring& filePath)
{
	QFile file(QString::fromStdWString(filePath));
	if (!file.open(QFile::ReadOnly)) {
		return false;
	}

	QStringList listDetail;
	QStringList listCatagory;
	while (!file.atEnd()) {
		QString line = file.readLine().trimmed();
		QStringList itemList = line.split(QStringLiteral("\t"));
		if (itemList.size() != 3) {
			continue;
		}
		const QString& number = itemList.at(0); //第一列是编号
		const QString& parentNumber = itemList.at(1); //第二列是父编号
		const QString& itemText = itemList.at(2); //第三列是详细说明
		QString numberText = number + QStringLiteral(" ") + itemText;
		listDetail << (numberText);
		if (parentNumber.isEmpty()) {
			listCatagory << numberText;
		}
		_parentMap[number] = parentNumber;
	}
	file.close();
	_detailModel = new QStringListModel(listDetail);
	_catagoryModel = new QStringListModel(listCatagory);
    return true;
}

QString CPCaseTypeHandler::getCatagory(const QString& numberText)
{
	QStringList itemList = numberText.trimmed().split(QStringLiteral(" "));
	if (itemList.isEmpty()) {
		return "";
	}
	const QString& number = itemList.at(0);

	QString parentNumber;
	auto it = _parentMap.find(number);
	while (it != _parentMap.end()) {
		if (it->second.isEmpty()) {
			break;
		}
		parentNumber = it->second;
		it = _parentMap.find(parentNumber);
	}
	if (parentNumber.isEmpty()) {
		return number;
	}
	return parentNumber;
}
