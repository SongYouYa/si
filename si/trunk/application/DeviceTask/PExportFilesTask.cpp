#include "stdafx.h"
#include "application/DeviceTask/PExportFilesTask.h"

#include "./fileoper/PLogger.h"
#include "./fileoper/PFileHandleHelper.h"

#include "sicommon/PPath.h"
#include "sicommon/PProcess.h"
#include "siutility/PDirHelper.h"
#include "siutility/PFileSiUtils.h"

#include "filesystemshell/FileSystemShell.h"
#include "base/PRecordType.h"
#include "base/PRecordTypeFactory.h"

#include "PReportFlags.h"

using namespace pdevice;
using namespace filesystem;

static CPRecord* s_exportFileRecord = nullptr;
static CPRecord* s_exportFileListRecord = nullptr;

namespace devtask
{
	void getItemFormatedOutPath(const std::wstring& parentDir, PSIHANDLE handle,
		std::wstring& targetPath)
	{
		//在此之前的都是handler 通过hangder获取文件名字
		std::wstring itemName = CFileSystem::GetItemName(handle);
		//冒号去掉
		boost::replace_all(itemName, L":", L"");
		if (itemName.empty()) {
			itemName = L"_";
		}
		targetPath = CPPath::getNewPath(parentDir + L"/" + itemName);
		CPPath::formatPathSlash(targetPath);
	}

	class CPExportFilesTask::Impl
	{
	public:
		Impl()
			: _totalSize(0), _curSize(0),
			_logger(nullptr), _errorLogger(nullptr)
		{

		}

		bool calcSize(const FileExportParam& param, CPTaskStatusCtrl* statusCtrl);

		bool exportWithStructure(PSIHANDLE handle, const std::wstring& parentDir,
			CPTaskStatusCtrl* statusCtrl, CPRecord* fileFolder);

		void exportNoStructure(const std::vector<PSIHANDLE>& handles, const std::wstring& outDir,
			CPTaskStatusCtrl* statusCtrl, CPRecord* fileFolder);
		//导出单个文件
		bool doExportFile(PSIHANDLE handle, const std::wstring& targetPath,
			CPTaskStatusCtrl* statusCtrl, CPRecord* fileFolder);

		bool initLogger(CPTaskStatusCtrl* statusCtrl, CPRecord* listFolder,
			std::wstring& fileListPath, CPRecord*& listRecord);
		void exportFiles(const FileExportParam& exportParam,
			CPTaskStatusCtrl* statusCtrl, CPRecord* fileFolder, CPRecord* listFolder);
		//只导出文件列表时
		void exportFileList(const FileExportParam& exportParam,
			CPTaskStatusCtrl* statusCtrl, CPRecord* listFolder);


		CPLogger* _logger;
		CPLogger *_errorLogger;
		std::wstring _outDir;
		std::wstring _errorDir;

		FileExportParam _exportParam;

		uint64 _totalSize;
		uint64 _curSize; //当前导出的大小
		ParentSubHandles _mapHandles; //导出目录结构时用
		std::vector<PSIHANDLE> _checkedHandles; //不导出目录结构时用
	};

	bool CPExportFilesTask::Impl::calcSize(const FileExportParam& param,
		CPTaskStatusCtrl* statusCtrl)
	{
		if (param.withStructure) { //默认是导出有目录结构的
			CPFileHandleHelper helper(_mapHandles);
			helper.update(param.rootHandles);
			_totalSize = helper.getTotalSize();
		}
		else {  //保留导出不带有目录结构的
			assert(false);
#if 0
			//TODO:判断是否选中
			for (auto it = param.rootHandles.begin(); it != param.rootHandles.end(); ++it) {
				//不进行过滤
				CFileSystem::GetAllSubItems(*it, _checkedHandles, true, L"");
			}
			for (auto it = _checkedHandles.begin(); it != _checkedHandles.end(); ++it) {
				_totalSize += CFileSystem::GetItemSize(*it);
			}
#endif
		}

		//把选中的文件取消选中，以免重复导出
		for (auto rootHandle : param.rootHandles) {
			CFileSystem::SetItemChecked(rootHandle, false);
		}
		statusCtrl->sendStage(FE_STATE_HANDLE_PARSED);

		try {
			//计算剩余空间
			boost::filesystem::space_info spaceInfo = boost::filesystem::space(param.exportDir);
			if (spaceInfo.available < _totalSize) {
				statusCtrl->fail(TASK_ERROR_SPACE_INSUFFICIENT);
				return false;
			}
		}
		catch (...) {
			statusCtrl->fail(TASK_ERROR_SPACE_INSUFFICIENT);
			return false;
		}
		
		//导出文件任务完成后压缩任务还没完成延长完成时间
		_totalSize += _totalSize / 10;
		return true;
	}

	bool CPExportFilesTask::Impl::exportWithStructure(PSIHANDLE handle, const std::wstring& parentDir,
		CPTaskStatusCtrl* statusCtrl, CPRecord* fileFolder)
	{
		std::wstring targetPath;
		getItemFormatedOutPath(parentDir, handle, targetPath);
		doExportFile(handle, targetPath, statusCtrl, fileFolder);
		ITEM_TYPE itemType = CFileSystem::GetItemType(handle);
		auto itp = _mapHandles.find(handle);
		if (itp != _mapHandles.end()) {
			std::vector<PSIHANDLE>& children = itp->second;
			for (auto itc = children.begin(); itc != children.end(); ++itc) {
				PSIHANDLE childHandle = *itc;
				bool ret = exportWithStructure(childHandle, targetPath, statusCtrl, fileFolder);
				if (!ret) {
					return false;
				}
			}
		}
		return true;
	}

	void CPExportFilesTask::Impl::exportNoStructure(const std::vector<PSIHANDLE>& handles,
		const std::wstring& outDir, CPTaskStatusCtrl* statusCtrl, CPRecord* fileFolder)
	{
		for (auto itc = handles.begin(); itc != handles.end(); ++itc) {
			if (statusCtrl && statusCtrl->isCanceled()) {
				break;
			}
			PSIHANDLE handle = *itc;
			std::wstring targetPath;
			getItemFormatedOutPath(outDir, handle, targetPath);
			//遍历一个一个文件进行导出
			bool ret = doExportFile(handle, targetPath, statusCtrl, fileFolder);
			if (!ret) {
				PLOG(PLOG_ERROR, L"导出无目录结构文件失败");
				break;
			}
		}
	}

	bool CPExportFilesTask::Impl::doExportFile(PSIHANDLE handle, const std::wstring& targetPath,
		CPTaskStatusCtrl* statusCtrl, CPRecord* fileFolder)
	{
		std::wstring itemName = CFileSystem::GetItemName(handle);
		CPTime createTime = CFileSystem::GetItemCreateTime(handle);
		CPTime modifyTime = CFileSystem::GetItemModfiyTime(handle);
		CPTime accessTime = CFileSystem::GetItemLastAccessTime(handle);
		//添加案件信息
		CPRecord* childRecord = fileFolder->addChild();
		childRecord->at(L"filename") = itemName;
		childRecord->at(L"create_time") = createTime;
		childRecord->at(L"modify_time") = modifyTime;
		childRecord->at(L"access_time") = accessTime;
		std::wstring formatedPath = targetPath;
		CPPath::formatPathSlash(formatedPath);
		childRecord->at(L"filedir") = formatedPath;

		std::string md5;
		std::wstring itemPath = CFileSystem::GetItemPath(handle);

		bool ret = siutil::CPSiFileUtils::exportFile(handle, targetPath, &md5,
			statusCtrl, &_totalSize, &_curSize);
		//如果导出成功写入日志或报告操作
		if (ret){
			_logger->add(itemName, itemPath, md5, createTime.ToString(),
				modifyTime.ToString(), accessTime.ToString());
		}
		else {
			//如果是手动终止的话就直接返回错误退出而不进行错误拷贝文件。
			if (statusCtrl && statusCtrl->isCanceled()) {
				return false;
			}
			if (!_errorLogger){
				_errorLogger = new CPLogger(_outDir + L"/errorlog.txt");
				if (!_errorLogger->init()) {
					PLOG(PLOG_ERROR, L"生成错误日志文件失败");
					statusCtrl->fail();          //只要初始化一次就ok了
					return false;
				}
			}
			//失败了再一次的进行导出，计算累加导出文件的大小
			std::wstring targetPath = CPPath::getNewPath(_errorDir + L"/" + itemName);
			ret = siutil::CPSiFileUtils::exportFile(handle, targetPath, &md5,
				statusCtrl, &_totalSize, &_curSize);
			if (ret) {
				_errorLogger->add(itemName, itemPath, md5, createTime.ToString(),
					modifyTime.ToString(), accessTime.ToString());
			}
			else {
				if (statusCtrl && statusCtrl->isCanceled()) {
					return false;
				}
			}
		}
		return true;
	}

	bool CPExportFilesTask::Impl::initLogger(CPTaskStatusCtrl* statusCtrl,
		CPRecord* listFolder, std::wstring& fileListPath, CPRecord*& listRecord)
	{
		fileListPath = _outDir + L"/FileList.txt";
		_logger = new CPLogger(fileListPath);
		if (!_logger->init()) {
			PLOG(PLOG_ERROR, L"生成日志文件失败");
			statusCtrl->fail();
			return false;
		}

		//添加文件列表record
		listRecord = listFolder->addChild();
		listRecord->at(L"filename") = L"FileList.txt";
		listRecord->at(L"export_time") = CPTime::GetCurrentTime();
		listRecord->at(L"filepath") = fileListPath;
		return true;
	}

	void CPExportFilesTask::Impl::exportFiles(const FileExportParam& exportParam,
		CPTaskStatusCtrl* statusCtrl, CPRecord* fileFolder, CPRecord* listFolder)
	{
		if (!calcSize(exportParam, statusCtrl)) {
			return;
		}
		//设置大小更新间隔长一点，速度可以计算更稳定一些
		statusCtrl->progress()->setUpdatePeriod(1500);
		statusCtrl->progress()->setTotalSize(_totalSize);
		//获取导出的文件路径

		_errorDir = _outDir + L"/ErrorFiles";
		//列表文件总是生成
		std::wstring fileListPath;
		CPRecord* listRecord = nullptr;
		if (!initLogger(statusCtrl, listFolder, fileListPath, listRecord)) {
			return;
		}

		for (auto it = exportParam.rootHandles.begin(); it != exportParam.rootHandles.end(); ++it) {
			bool ret = exportWithStructure(*it, _outDir, statusCtrl, fileFolder);
			if (!ret) {
				break;
			}
		}
		_logger->close();
		//导出文件完成进行计算大小
		try  {
			boost::filesystem::path filepath(fileListPath);
			uint64 fileSzie = boost::filesystem::file_size(filepath);
			listRecord->at(L"size") = fileSzie;
		}
		catch (...)
		{
		}

		//任务取消的时候停止打包
		if (!statusCtrl->isCanceled()){
			if (exportParam.createZip) {

				std::wstring zipFilePath = _outDir + L".7z";
				zipFilePath = CPPath::getNewPath(zipFilePath);
                std::wstring zipToolPath = siutil::CPDirHelper::get()->getToolDir();
#ifdef OS_PLINUX
				zipToolPath += L"/zip/7za ";
				std::wstring cmd = zipToolPath + L"a -t7z " + L"\"" + zipFilePath + L"\" " + L"\"" + _outDir + L"\"";
#else
				zipToolPath = zipToolPath + L"/zip/7z.exe ";
				std::wstring cmd = zipToolPath + L"a -tzip " + L"\"" + zipFilePath + L"\" " + L"\"" + _outDir + L"\"";
#endif
                int exitCode = 0;
				bool ret = CPProcess::createProcess(cmd, L"", NULL, &exitCode);
                if (!ret || exitCode != 0){
					PLOG(PLOG_ERROR, L"导出压缩文件失败");
					statusCtrl->fail();
				}
			}
			statusCtrl->progress()->updateSize(_totalSize);
			statusCtrl->progress()->updateProgress(1000);
		}
		return;
	}

	void CPExportFilesTask::Impl::exportFileList(const FileExportParam& exportParam,
		CPTaskStatusCtrl* statusCtrl, CPRecord* listFolder)
	{
		std::wstring fileListPath;
		CPRecord* listRecord = nullptr;
		if (!initLogger(statusCtrl, listFolder, fileListPath, listRecord)) {
			return;
		}

		for (auto it = exportParam.rootHandles.begin(); it != exportParam.rootHandles.end(); ++it) {
			std::vector<PSIHANDLE> tempHandles;
			CFileSystem::GetAllSubItems(*it, tempHandles, true);
			for (auto handle : tempHandles) {
				ITEM_CHECK_STATE state = CFileSystem::ItemCheckState(handle);
				if (state != ITEM_CHECK_NONE &&
					!CFileSystem::IsFolder(handle)) {
					_checkedHandles.push_back(handle);
				}
			}
		}

		CPTaskProgress* progress = statusCtrl->progress();
		uint64 totalCount = _checkedHandles.size();
		uint64 curCount = 0;
		for (auto handle : _checkedHandles) {
			if (statusCtrl->isCanceled()) {
				break;
			}
			std::wstring itemName = CFileSystem::GetItemName(handle);
			std::wstring itemPath = CFileSystem::GetItemPath(handle);
			CPTime createTime = CFileSystem::GetItemCreateTime(handle);
			CPTime modifyTime = CFileSystem::GetItemModfiyTime(handle);
			CPTime accessTime = CFileSystem::GetItemLastAccessTime(handle);
			_logger->add(itemName, itemPath, "", createTime.ToString(),
				modifyTime.ToString(), accessTime.ToString());
			curCount++;
			progress->updateProgress(int(curCount * 1000 / totalCount));
		}
		_logger->close();

		try  {
			boost::filesystem::path filepath(fileListPath);
			uint64 fileSzie = boost::filesystem::file_size(filepath);
			listRecord->at(L"size") = fileSzie;
		}
		catch (...)
		{
		}
	}

	CPExportFilesTask::CPExportFilesTask()
		: _impl(nullptr)
	{

	}

	CPExportFilesTask::~CPExportFilesTask()
	{
		if (_impl) {
			delete _impl;
			_impl = nullptr;
		}
	}

	void CPExportFilesTask::doTask(CPTask* task)
	{
		_impl = new Impl();

		TaskParamPtr taskParam = task->param();
		FileExportParam exportParam = boost::any_cast<FileExportParam>(*taskParam);
		CPTaskStatusCtrl* statusCtrl = task->statusCtrl();

		//获取导出的文件路径
		_impl->_outDir = exportParam.exportDir + L"/ExportFiles";
		_impl->_outDir = CPPath::getNewPath(_impl->_outDir);
		CPPath::formatPathSlash(_impl->_outDir);

		CPRecord* folderRecord = exportParam.folderRecord;
		if (!s_exportFileListRecord) {
			s_exportFileListRecord = CPRecordTypeFactory::get()->getType(L"table.fileexportlist")->newRecord();
			folderRecord->addChild(s_exportFileListRecord);
		}

		if (exportParam.exportFile) {
			if (!s_exportFileRecord) {
				s_exportFileRecord = CPRecordTypeFactory::get()->getType(L"table.fileexport")->newRecord();
				folderRecord->addChild(s_exportFileRecord);
			}
			_impl->exportFiles(exportParam, statusCtrl, s_exportFileRecord, s_exportFileListRecord);
		}
		else {
			_impl->exportFileList(exportParam, statusCtrl, s_exportFileListRecord);
		}
	}
}

