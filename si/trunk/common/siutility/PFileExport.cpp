#include "stdafx.h"
#include "siutility/FileSearch.h"
#include "siutility/PFileExport.h"
#include "siutility/PFileSiUtils.h"
#include "siutility/PDirHelper.h"
#include "filesystemshell/FileSystemShell.h"

namespace siutil{
    CPHandleExport::CPHandleExport()
    {
    }

    CPHandleExport::~CPHandleExport()
    {
        Delete();
    }
    std::wstring CPHandleExport::getOutRootPath()
    {
       if(outRootPath_.empty())
       {
           std::wstring caseTemp = siutil::CPDirHelper::get()->getTempDir();
           std::wstring uuidstr = CPUtils::getGuid();
           outRootPath_ = caseTemp +L"/" +uuidstr;
           try {
               boost::filesystem::create_directories(outRootPath_);
           }
           catch(...)
           {
               return L"";
           }
       }
       return outRootPath_;
    }
    std::wstring CPHandleExport::Export(PSIHANDLE fileHandle,CPTaskStatusCtrl *pCtrl)
    {
        if(fileHandle == NULL){
            return L"";
        }
        ITEM_TYPE type = filesystem::CFileSystem::GetItemType(fileHandle);
        if (type != ITEM_TYPE_FILE){
            return L"";
        }
        std::wstring  outRootPath = getOutRootPath();
        std::wstring fileName = filesystem::CFileSystem::GetItemName(fileHandle);
        std::wstring uuidstr=CPUtils::getGuid();
        fileName = uuidstr + L"-" + fileName;           
        std::wstring outPath = outRootPath+ L"/" +fileName;
        siutil::CPSiFileUtils::exportFile(fileHandle,outPath) ;
        return  outPath;
    }
    void CPHandleExport::Export(std::vector<PSIHANDLE> handles, std::vector<std::wstring>& outRealPaths, CPTaskStatusCtrl* pCtrl)
    {
        for(std::vector<PSIHANDLE>::iterator it = handles.begin(); it != handles.end(); ++it)
        {
           std::wstring outPath = Export(*it, pCtrl);
           outRealPaths.push_back(outPath);
        }
    }
    void CPHandleExport::exportHandlesToDesPath(std::vector<PSIHANDLE> handles, const std::wstring  &rootPath,CPTaskStatusCtrl *pCtrl)
    {
        for(std::vector<PSIHANDLE>::iterator it = handles.begin(); it != handles.end(); ++it)
        {
           exportHandleToRootpath(*it,rootPath,pCtrl);
        }
    }
    bool CPHandleExport::exportHandleToRootpath(PSIHANDLE fileHandle, const std::wstring wstrDesPath, CPTaskStatusCtrl *pCtrl)
       {
           std::wstring name = filesystem::CFileSystem::GetItemName(fileHandle);
           std::wstring struuid = CPUtils::getGuid();
           struuid += L"_" + name;
           std::wstring outPath = wstrDesPath + L"/"+ struuid;
           siutil::CPSiFileUtils::exportFile(fileHandle,outPath) ;
            if(outPath.empty()){
                return false;
            }
           return true;
       }

    void CPHandleExport::Delete()
    {
        try
        {
            boost::filesystem::remove_all(outRootPath_);
        }
        catch(...)
        {
        }
    }

    CPFileExport::CPFileExport(const std::wstring rawRootPath, const std::wstring fileName)
        : rawRootPath_(rawRootPath), fileName_(fileName)
    {

    }

    CPFileExport::~CPFileExport()
    {

    }

    void CPFileExport::getExportFiles(std::vector<std::wstring>& exportFiles)
    {
        if(exportedFiles_.empty())
        {
            std::vector<PSIHANDLE> targetHandles;
            std::wstring targetRegex = fileName_ + L"$"; //用户名正则
            CFileSearch cfs(rawRootPath_, targetRegex);
            cfs.GetHandles(targetHandles);
            exportTargets(targetHandles);

        }
         exportFiles = exportedFiles_;
    }

    void CPFileExport::exportTargets(std::vector<PSIHANDLE>& targetHandles)
    {
        // 分别导出每个目标文件
        for(std::vector<PSIHANDLE>::iterator it_t = targetHandles.begin(); it_t != targetHandles.end(); ++it_t)
        {
            PSIHANDLE pOneTarget = *it_t;
            std::vector<PSIHANDLE> attachedHandles;
            getAttachedHandles(pOneTarget, attachedHandles);
            CPHandleExportPtr chxptr(new CPHandleExport);

            std::vector<std::wstring> attoutpaths;
            chxptr->Export(attachedHandles,attoutpaths);
            std::wstring outPath = chxptr->Export(pOneTarget);
            chxptrs_.push_back(chxptr); //备忘录模式,哪还原信息?
            exportedFiles_.push_back(outPath); // 返回导出的目标文件
        }
    }
    void CPFileExport::getAttachedHandles(PSIHANDLE itemHandle, std::vector<PSIHANDLE>& attachedHandles)
    {
        PSIHANDLE itemParent = filesystem::CFileSystem::GetItemParent(itemHandle);
        int subCount = filesystem::CFileSystem::SubItemCount(itemParent);
        for(int i = 0; i != subCount; ++i)
        {
            PSIHANDLE subItem = filesystem::CFileSystem::GetSubItemHandle(itemParent, i);
            std::wstring itemName = filesystem::CFileSystem::GetItemName(subItem);
            if(itemName.find(fileName_) != std::wstring::npos && subItem != itemHandle)
            {
                attachedHandles.push_back(subItem);
            }
        }
    }

}
