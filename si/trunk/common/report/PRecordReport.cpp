#include "stdafx.h"
#include "report/PRecordReport.h"
#include "PReportPath.h"
#include "handler/PReportHandler.h"
#include "material/PTree.h"
#include "material/PPageSet.h"


extern std::wstring g_productName;

namespace report
{
	CPRecordReport::CPRecordReport(CPRecord* rootRecord)
		: _rootRecord(rootRecord),
		_withMedia(false)
	{

	}

	CPRecordReport::~CPRecordReport()
	{

	}

	std::wstring CPRecordReport::createReport(const std::wstring& outDir, bool withMedia)
	{
		_withMedia = withMedia;

        std::wstring dirPath = outDir; //CPPath::getNewPath(outDir);
		_rootDir = dirPath;

		//拷贝css、js等文件
		CPPath::copyFiles(CPReportPath::GetHtmlConfDir() + L"/list", dirPath);
		CPPath::copyFiles(CPReportPath::GetHtmlConfDir() + L"/tree", dirPath);

		
		std::wstring mainIndexPath = dirPath + L"/index.html";
		TemplateDictionary mainDict("");
		CPReportHandler::outputTemplate(&mainDict, L"main.tpl", mainIndexPath);

		std::wstring headerPath = dirPath + L"/sub/header.html";
		TemplateDictionary headerDict("");
		headerDict.SetValue("HEADER_TEXT", W2U8(g_productName) + GET_TEXT(TEXT_ID_HTML_HEADER));
		CPReportHandler::outputTemplate(&headerDict, L"header.tpl", headerPath);

		std::wstring treePath = dirPath + L"/sub/tree.html";
		CPTree tree(_rootRecord);
		tree.output(treePath);

		std::wstring contentDir = dirPath + L"/sub";
		createContentPages(_rootRecord, contentDir, 1);

		return mainIndexPath;
	}

	void CPRecordReport::createContentPages(CPRecord* parentRecord, const std::wstring& outDir, int level)
	{
		if (parentRecord->flags() & RFlagNoReport) {
			return;
		}
		if (parentRecord->childrenCount() == 0) {
			return;
		}
		const auto& children = parentRecord->getChildren();
		bool isChildrenFolder = children.at(0)->isFolder();
		if (isChildrenFolder) {
			//没有分析的目录项，不生成在报告里
			std::vector<CPRecord*> parsedChildren;
			for (auto child : children) {
				if (!(child->flags() & RFlagNoReport)) {
					parsedChildren.push_back(child);
				}
			}
			CPPageSet pages(parsedChildren, outDir, level, _rootDir, _withMedia);
			pages.output();
			for (size_t i = 0; i < parsedChildren.size(); ++i) {
				CPRecord* child = parsedChildren.at(i);
				std::wstring childOutDir = outDir + L"/" + boost::lexical_cast<std::wstring>(i);
				createContentPages(child, childOutDir, level + 1);
			}
		}
		else {
			CPPageSet pages(children, outDir, level, _rootDir, _withMedia);
			pages.output();
		}
	}
}
