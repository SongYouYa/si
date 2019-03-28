#include "stdafx.h"

#include "base/globals.h"
#include "base/PTime.h"
#include "base/PRecord.h"
#include "base/PRecordType.h"

#include "base/PRecordTypeFactory.h"

#include "report/PReportConfig.h"
#include "report/PRecordReport.h"
#include "report/PRecordReport2.h"

using namespace report;

#pragma comment(lib, "base.lib")
#pragma comment(lib, "report.lib")

void report_init()
{
	std::wstring appRootDir = CPUtils::getAppRootDir();

	const std::wstring confDir = L"conf";
	std::wstring confDirPath = appRootDir + L"\\" + confDir;
	std::wstring tableXml = confDirPath +L"\\table.xml";
	CPRecordTypeFactory::get()->loadXml(tableXml);
	std::wstring tableWinXml = confDirPath + L"\\table_win.xml";
	CPRecordTypeFactory::get()->loadXml(tableWinXml);
	
	SetConfRootDir(appRootDir + L"\\resource\\report");
}

CPRecord* create_record()
{
	CPRecordTypeFactory* typeFact = CPRecordTypeFactory::get();
	CPRecord* rootRecord = typeFact->getType(L"table.case")->newRecord();
	
	CPRecord* caseInfoFolder = typeFact->getType(L"table.caseinfo")->newRecord();
	for (int i = 0; i < 200; ++i) {
		CPRecord* caseInfo = caseInfoFolder->addChild();
		caseInfo->at(L"case_name") = L"中文案件1";
		caseInfo->at(L"real_time") = CPTime::GetCurrentTime();
	}
	rootRecord->addChild(caseInfoFolder);

	CPRecord* appFolder = typeFact->getType(L"table.approot")->newRecord();
	CPRecord* sysFolder = typeFact->getType(L"table.systemfolder")->newRecord();
	CPRecord* osInfoFolder = typeFact->getType(L"table.win.osinfo")->newRecord();
	for (int i = 0; i < 201; ++i) {
		CPRecord* osInfo = osInfoFolder->addChild();
		osInfo->at(L"ComputerName") = L"Windows8 系统";
	}
	sysFolder->addChild(osInfoFolder);
	appFolder->addChild(sysFolder);

	rootRecord->addChild(appFolder);
	return rootRecord;
}

void create_report1(CPRecord* rootRecord)
{
	std::wstring appDir = CPUtils::getAppDir();
	std::wstring htmlDir = appDir + L"\\report\\r";

	CPRecordReport reporter(rootRecord);
	std::wstring htmplPath = reporter.createReport(htmlDir, false);
}

void create_report2(CPRecord* rootRecord)
{
	std::wstring appDir = CPUtils::getAppDir();
	std::wstring htmlDir = appDir + L"\\report\\r2";

	CPRecordReport2 reporter(rootRecord);
	std::wstring htmplPath = reporter.createReport(htmlDir);
}

void test_report()
{
	report_init();
	CPRecord* rootRecord = create_record();
	//create_report1(rootRecord);
	create_report2(rootRecord);
	std::string temp;
}