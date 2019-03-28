#ifndef TOOLS_FILEEXPORT_H
#define TOOLS_FILEEXPORT_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "task/PTaskStatusCtrl.h"

namespace siutil
{
    class  CPHandleExport
    {
    public:
        CPHandleExport();
        ~CPHandleExport();

        static void exportHandlesToDesPath(std::vector<PSIHANDLE> handles, const std::wstring  &rootPath,CPTaskStatusCtrl *pCtrl= NULL);
        //导出多个handle到指定根路径下，前面加hash值以区别
        static bool exportHandleToRootpath(PSIHANDLE fileHandle, const std::wstring wstrDesPath, CPTaskStatusCtrl *pCtrl = NULL);

        //===========================================================
        // brief :		导出一个文件到SI U盘临时目录里 (可多次调用，同一个对象导出的文件都在同一个目录里)
        // param		[输入]：handle : 要导出的文件的handle
        // Returns:	std::wstring : 实际导出的路径
        //===========================================================
        std::wstring Export(PSIHANDLE fileHandle,CPTaskStatusCtrl* pCtrl = NULL);

        // 导出容器里所有handle，返回对应的导出路径
        void Export(std::vector<PSIHANDLE> handles, std::vector<std::wstring>& outRealPaths,CPTaskStatusCtrl* pCtrl = NULL);

    private:
        // 获取导出文件的目录路径
        std::wstring getOutRootPath();
        //删除导出目录及其下所有文件
        void Delete();

        std::wstring outRootPath_; //导出的文件所在目录的路径
    };

    typedef boost::shared_ptr<CPHandleExport> CPHandleExportPtr;
    typedef std::vector<CPHandleExportPtr> CPHandleExportPtrs;

    class  CPFileExport
    {
    public:
        //===========================================================
        // brief :		在卷目录下的某个目录下，查找目标文件名（History.db），
        //           并导出该文件及其依赖文件（如History.db-war、History.db-shm）
        // param		[输入]：rawRootPath : 目标文件所在某个上层目录, 在该目录下递归查找目标文件
        // param		[输入]：fileName : 目标文件名
        //===========================================================
        CPFileExport(const std::wstring rawRootPath, const std::wstring fileName);
        // 析构时，会删除导出文件
        ~CPFileExport();

        // 获取所有导出的目标文件的绝对路径
        void getExportFiles(std::vector<std::wstring>& exportFiles);

    private:
        void exportTargets(std::vector<PSIHANDLE>& targetHandles);
        void getAttachedHandles(PSIHANDLE itemHandle, std::vector<PSIHANDLE>& attachedHandles);

        std::wstring rawRootPath_;
        std::wstring fileName_;
        CPHandleExportPtrs chxptrs_; //保存所有的文件导出类
        std::vector<std::wstring> exportedFiles_;
    };
    typedef boost::shared_ptr<CPFileExport> CPFileExportPtr;
    typedef std::vector<CPFileExportPtr> CFileExportPtrs;
}

#endif // TOOLS_FILEEXPORT_H
