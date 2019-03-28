#include "stdafx.h"
#include "report/PRecordReport2.h"
#include "handler/PReportHandler.h"
#include "material/PContent2.h"
#include "material/PTree2.h"
#include "PReportPath.h"

namespace report
{
	extern const std::wstring ContentHtml2 = L"content.html";

	CPRecordReport2::CPRecordReport2(CPRecord* rootRecord)
		: _rootRecord(rootRecord)
	{

	}

	std::wstring CPRecordReport2::createReport(const std::wstring& outDir)
	{
		std::wstring dirPath = outDir; //CPPath::getNewPath(outDir);
		_rootDir = dirPath;

		//¿½±´css¡¢jsµÈÎÄ¼þ
		CPPath::copyFiles(CPReportPath::GetHtmlConfDir() + L"/tree", dirPath);

		std::wstring mainIndexPath = dirPath + L"/index.html";
		TemplateDictionary mainDict("");
		CPReportHandler::outputTemplate(&mainDict, L"main2.tpl", mainIndexPath);

		std::wstring treePath = dirPath + L"/tree.html";
		CPTree2 tree(_rootRecord);
		tree.output(treePath);

		std::wstring tablesPath = dirPath + L"/" + ContentHtml2;
		CPContent2 tables(_rootRecord);
		tables.output(tablesPath);

		return mainIndexPath;
	}
}