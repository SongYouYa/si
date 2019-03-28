#include "stdafx.h"
#include "PLogListAnalyzer.h"
#include "TemplateFile/PTemplateFile.h"
#include "../../../si/trunk/common/TemplateFile/TemplateApp.h"
using namespace tpfile;

namespace macbasic{
CPlogListAnalyzer::CPlogListAnalyzer()
{

}

bool CPlogListAnalyzer::getFileListInfos(CPRecord *rootRecord)
{
    std::wstring typeName = rootRecord->getType()->name();
    std::wstring confDir = CPDirHelper::get()->getConfDir();
    if(typeName == L"table.mac.LogFileInfo"){
        std::wstring xmlPath = confDir + L"/MacAppTemplate/TaskLogFileExport.xml";
        return analyzeFileInfo(rootRecord, xmlPath);
    }else if(typeName == L"table.mac.UserFileList") {
        std::wstring xmlPath = confDir + L"/MacAppTemplate/TaskUserFileList.xml";
        return analyzeFileInfo(rootRecord, xmlPath);
    }
    return false;
}

CPlogListAnalyzer::~CPlogListAnalyzer()
{
}

bool CPlogListAnalyzer::analyzeFileInfo(CPRecord *rootRecord,const std::wstring &xmlpath)
{
    CTemplateApp tapp;
    if (!tapp.ParseTemplate(xmlpath)) {
        return false;
    }
    if (!tapp.SearchFiles()) {
        return false;
    }
    std::vector<PSIHANDLE> vhandles;
    tapp.GetSearchedHandles(vhandles);
    std::vector<PSIHANDLE>::iterator it = vhandles.begin();
    for (; it != vhandles.end(); ++it)
    {
        PSIHANDLE realHandle = *it;
        if (CFileSystem::IsFolder(realHandle)) //文件夹不存
        {
            continue;
        }

        siutil::FileItemInfo fileInfo;
        siutil::CPFileOperate::ParseFileInfo(realHandle, fileInfo, true); //此处解析lnk实际路径
        fileInfo.AddToTable(rootRecord);
    }
    return true;
}

}
