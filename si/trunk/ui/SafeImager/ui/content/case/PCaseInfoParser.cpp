#include "stdafx.h"
#include "PCaseInfoParser.h"

#ifdef OS_PWINDOWS
#include "siwinregistry/PRegistry.h"
#include "PRegistryValue.h"
#endif

#include "base/PRecordTypeFactory.h"
#include "siio/PNormalFile.h"


extern QString PTIME_FORMAT;

const QString CASE_INVESTIGATOR = "investigator";
const QString CASE_NUMBER = "number";
const QString CASE_YEAR = "year";

inline std::wstring getCaseIni()
{
	std::wstring confDir = CPAppContext::get()->opath(L"conf");
	return confDir + L"/case.ini";
}

CPCaseInfoParser::CPCaseInfoParser()
{

}

void CPCaseInfoParser::getCaseInfo(PCaseInfo& caseInfo)
{
	std::wstring caseIni = getCaseIni();
	QSettings setting(QString::fromStdWString(caseIni), QSettings::IniFormat);
	setting.beginGroup("case");
	QDateTime currentTime = QDateTime::currentDateTime();
	QString strTime = currentTime.toString("yyyyMMdd_hhmmss");
	caseInfo[PCASE_NAME] = strTime.toStdWString();
	caseInfo[PCASE_INV] = setting.value(CASE_INVESTIGATOR).toString().toStdWString();
	caseInfo[PCASE_RT] = currentTime.toString(PTIME_FORMAT).toStdWString();
	setting.endGroup();

	std::wstring rootDir = CPCase::get()->caseRootDir();
	if (!rootDir.empty()) {
		std::wstring caseDir = rootDir + L"/" + strTime.toStdWString();
		CPPath::formatPathSlash(caseDir);
		caseInfo[PCASE_DIR] = caseDir;
	}
}

bool CPCaseInfoParser::getCaseInfo(const std::wstring& caseFile, PCaseInfo& caseInfo)
{
	QSettings infoSetting(QString::fromStdWString(caseFile), QSettings::IniFormat);
	infoSetting.beginGroup("caseinfo");
	QStringList itemKeys = infoSetting.childKeys();
	for (int i = 0; i < itemKeys.size(); ++i) {
		QString key = itemKeys.at(i);
		QString value = infoSetting.value(key).toString();
		caseInfo[key.toStdWString()] = value.toStdWString();
	}
	infoSetting.endGroup();
	if (caseInfo.empty()){
		return false;
	}
	else{
		return true;
	}
}

bool CPCaseInfoParser::saveCaseInfo(const PCaseInfo& caseInfo,
	const std::wstring& caseFile, bool updateCaseNum)
{
	QSettings infoSetting(QString::fromStdWString(caseFile), QSettings::IniFormat);
	infoSetting.beginGroup("caseinfo");
	for (auto& item : caseInfo) {
		infoSetting.setValue(QString::fromStdWString(item.first),
			QString::fromStdWString(item.second));
	}
	infoSetting.endGroup();
	infoSetting.sync();
	if (infoSetting.status() != QSettings::NoError) {
		return false;
	}

	//TODO:重新打开保存一下，确保缓存已写入磁盘

	std::wstring caseIni = getCaseIni();
	QSettings configSetting(QString::fromStdWString(caseIni), QSettings::IniFormat);
	configSetting.beginGroup("case");
	auto it = caseInfo.find(PCASE_INV);
	if (it != caseInfo.end()) {
		QString newInvestigator = QString::fromStdWString(it->second);
		QString cachedInvestigator = configSetting.value(CASE_INVESTIGATOR).toString();
		if (cachedInvestigator != newInvestigator) {
			configSetting.setValue(CASE_INVESTIGATOR, newInvestigator);
		}
	}

	if (updateCaseNum) {
		QString cachedNumber = configSetting.value(CASE_NUMBER).toString();
		QString newNumber = QString::number(cachedNumber.toInt() + 1);
		configSetting.setValue(CASE_NUMBER, newNumber);
	}
	configSetting.endGroup();
	configSetting.sync();
	if (configSetting.status() != QSettings::NoError) {
		return false;
	}
	return true;
}

CPRecordType* CPCaseInfoParser::getCaseType()
{
	return CPRecordTypeFactory::get()->getType(L"table.caseinfo");
}

void CPCaseInfoParser::caseInfo2Record(PCaseInfo& caseInfo, CPRecord* record)
{
	record->at(PCASE_NUMBER) = caseInfo[PCASE_NUMBER];
	record->at(PCASE_NAME) = caseInfo[PCASE_NAME];
	record->at(PCASE_TYPE) = caseInfo[PCASE_TYPE];
	record->at(PCASE_INV) = caseInfo[PCASE_INV];
	record->at(PCASE_VIT) = caseInfo[PCASE_VIT];
	record->at(PCASE_RT) = caseInfo[PCASE_RT];
	record->at(PCASE_ADD) = caseInfo[PCASE_ADD];
	record->at(PCASE_SUSPT) = caseInfo[PCASE_SUSPT];
	record->at(PCASE_REMARK) = caseInfo[PCASE_REMARK];
}
