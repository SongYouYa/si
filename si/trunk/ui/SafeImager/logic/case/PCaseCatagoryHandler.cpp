#include "stdafx.h"
#include "PCaseCatagoryHandler.h"


CPCaseCatagoryHandler* CPCaseCatagoryHandler::_instance = nullptr;

CPCaseCatagoryHandler* CPCaseCatagoryHandler::get()
{
	if (!_instance) {
		_instance = new CPCaseCatagoryHandler();
	}
	return _instance;
}

CPCaseCatagoryHandler::CPCaseCatagoryHandler()
{

}

bool CPCaseCatagoryHandler::loadIni(const std::wstring& filePath)
{
	_defaultApps.insert(L"table.diskinfo");
	return true;
}

void CPCaseCatagoryHandler::setCatagory(const QString& catagory)
{
	if (_catagory != catagory) {
		_catagory = catagory;
		emit signalCatagoryChanged();
	}
}

void CPCaseCatagoryHandler::getCatagoryApps(std::set<std::wstring>& apps)
{
	apps = _defaultApps;
}