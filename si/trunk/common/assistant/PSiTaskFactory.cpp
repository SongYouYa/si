#include "stdafx.h"
#include "assistant/PSiTaskFactory.h"

#include "libdevtask/PDeviceTaskParams.h"

#include "sidevice/PDeviceManager.h"
#include "siio/PHash.h"
#include "sifile/PMemoryReader.h"
#include "base/PRecordHandler.h"
#include "PReportFlags.h"

#include "application/DeviceTask/PDeviceCloneTask.h"
#include "application/DeviceTask/PDeviceImageTask.h"
#include "application/DeviceTask/PDeviceEraseTask.h"
#include "application/DeviceTask/PScreenRecordTask.h"
#include "application/DeviceTask/PExportFilesTask.h"

#include "application/AnalyzerAssistant/PAnalyzerAssistant.h"
#include "application/CommonAnalyzer/PAppParam.h"

using namespace pdevice;
using namespace siio;
using namespace app;
using namespace devtask;


std::wstring getImageFormatStr(ImageFormatOption::ImageType imageType)
{
	if (imageType == ImageFormatOption::IMAGE_DD) {
		return L"DD";
	}
	else if (imageType == ImageFormatOption::IMAGE_E01) {
		return L"E01";
	}
	else if (imageType == ImageFormatOption::IMAGE_AFF) {
		return L"AFF";
	}
    else if (imageType == ImageFormatOption::IMAGE_ZIP) {
        return L"zip";
    }
	return L"";
}

std::wstring getEraseTypeStr(DeviceEraseType type)
{
	if (type == ERASE_TYPE_SIMPLE) {
		return L"简单擦除";
	}
	else if (type == ERASE_TYPE_BMB) {
		return L"BMB";
	}
	else if (type == ERADE_TYPE_DOD) {
		return L"DOD";
	}
	return L"";
}

std::wstring getDeviceStr(const libdt::OneDevice& device)
{
	std::wstring deviceName;
	if (device.isPartition) {
		DevicePartition partition;
		CPDeviceManager::get()->getPartitionByName(device.deviceName, partition);
		deviceName = partition.displayName;
	}
	else {
		DeviceDisk disk;
		CPDeviceManager::get()->getDiskByName(device.deviceName, disk);
		deviceName = disk.displayName;
	}
	return deviceName;
}

void getDevicesStr(const std::vector<libdt::OneDevice>& devices,
	std::wstring& str)
{
	std::vector<std::wstring> deviceNames;
	for (auto& device : devices) {
		std::wstring deviceName = getDeviceStr(device);
		deviceNames.push_back(deviceName);
	}
	CPString::joinArray(deviceNames, L",", str);
	str += L"; ";
}

void getHashStr(const std::vector<siio::HashType>& hashTypes,
	std::wstring& str)
{
	if (hashTypes.empty()) {
		return;
	}
	str += L"校验方式: ";
	std::vector<std::wstring> hashs;
	for (auto type : hashTypes) {
		hashs.push_back(CPHash::getName(type));
	}
	CPString::joinArray(hashs, L",", str);
	str += L"; ";
}

void getCloneParamStr(const DeviceCloneParam& cloneParam, std::wstring& paramStr,
	CPRecord* record)
{
	paramStr += L"源磁盘: ";
	std::wstring sourceName = getDeviceStr(cloneParam.sourceDevice);
	record->at(L"source_media") = sourceName;
	paramStr += sourceName + L"; ";
	paramStr += L"目标磁盘: ";
	std::wstring targetName = getDeviceStr(cloneParam.targetDevice);
	record->at(L"target_media") = targetName;
	paramStr += targetName + L"; ";
}

void getImageCommonParamStr(const std::wstring& imagePath, const ImageFormatOption& formatOptions,
	const std::vector<siio::HashType>& hashTypes, std::wstring& paramStr, CPRecord* record)
{
	paramStr += L"目标路径: ";
	record->at(L"dst_path") = imagePath;
	paramStr += imagePath + L"; ";
	paramStr += L"镜像格式: ";
	paramStr += getImageFormatStr(formatOptions.imageType) + L"; ";
	record->at(L"image_type") = getImageFormatStr(formatOptions.imageType);
	paramStr += L"分片大小: ";
	std::wstring strSplit;
	if (formatOptions.splitSize > 0) {
		paramStr += CPUtils::formatSizeComma(formatOptions.splitSize);
		record->at(L"split_size") = formatOptions.splitSize;
	}
	else {
		paramStr += L"N/A; ";
		record->at(L"split_size") = L"N/A";
	}
	getHashStr(hashTypes, paramStr);
}

void getDeviceImageParamStr(const DeviceImageParam& imageParam, std::wstring& paramStr,
	CPRecord* record)
{
	paramStr += L"源磁盘: ";
	std::wstring sourceName = getDeviceStr(imageParam.sourceDevice);
	paramStr += sourceName + L"; ";
	record->at(L"source_media") = sourceName;
	getImageCommonParamStr(imageParam.imagePath, imageParam.formatOptions, 
		imageParam.hashTypes, paramStr, record);
}

#if (defined OS_PWINDOWS) || (defined OS_PAPPLE)
void getMemoryImageParamStr(const MemoryImageParam& imageParam, std::wstring& paramStr,
	CPRecord* record)
{
	paramStr += L"源磁盘: ";
	std::wstring sourceName = CPMemoryReader::getDisplayName();
	paramStr += sourceName + L"; ";
	record->at(L"source_media") = sourceName;
	getImageCommonParamStr(imageParam.imagePath, imageParam.formatOptions, 
		imageParam.hashTypes, paramStr, record);
}
#endif

void getEraseParamStr(const DeviceEraseParam& eraseParam, std::wstring& paramStr,
	CPRecord* record)
{
	paramStr += L"目标磁盘: ";
	std::wstring targetName = getDeviceStr(eraseParam.targetDevice);
	record->at(L"target_media") = targetName;
	paramStr += targetName + L"; ";
	paramStr += L"擦除方式: ";
	paramStr += getEraseTypeStr(eraseParam.eraseType) + L"; ";
}
void getScreenRecordParamStr(const VideoParam& recordParam, std::wstring& paramStr,
	CPRecord* record)
{
	paramStr += L"录像保存路径: ";
	paramStr += recordParam.videoDirectorPath;
	record->at(L"recordVideopath") = recordParam.videoDirectorPath;
}

void getFileExportParamStr(const TaskParamPtr param, std::wstring& paramStr)
{
	FileExportParam exportParam = boost::any_cast<FileExportParam>(*param);
	paramStr += L"导出文件路径: ";
	paramStr += exportParam.exportDir;
}

//获取选中的分析项类型
void getCheckedApps(CPRecord* record, std::vector<std::wstring>& apps)
{
	if (record->getCheckState() == RECORD_CHECK_NONE) {
		return;
	}
	if (record->childrenCount() == 0 &&
		!(record->flags() & RFlagAppParsed)) {
		apps.push_back(record->getType()->display());
		return;
	}
	for (auto child : record->getChildren()) {
		if (!child->isFolder()) {
			break;
		}
		getCheckedApps(child, apps);
	}
}

void getAppParamStr(const TaskParamPtr param, std::wstring& paramStr)
{
	paramStr += L"分析项: ";
	AppParam appParam = boost::any_cast<AppParam>(*param);
	CPRecord* rootRecord = appParam.rawRootRecord;
	if (appParam.rootRecord) {
		rootRecord = appParam.rootRecord;
	}
	std::vector<std::wstring> checkedApps;
	getCheckedApps(rootRecord, checkedApps);
	CPString::joinArray(checkedApps, L",", paramStr);
}

void CPSiTaskFactory::getTaskParams(const std::wstring& taskType,
	const TaskParamPtr param,
	CPRecord* record,
	std::wstring& paramStr,
	TaskCallback& callback,
	CPTaskDelegatePtr& taskDelegate)
{
	if (taskType == L"task.deviceclone") {
		CPDeviceCloneTask cloneTask;
		callback = boost::bind(&CPDeviceCloneTask::doTask, cloneTask, _1);
		DeviceCloneParam cloneParam = boost::any_cast<DeviceCloneParam>(*param);
		getCloneParamStr(cloneParam, paramStr, record);
	}
	else if (taskType == L"task.deviceimage") {
		CPDeviceImageTask imageTask;
		callback = boost::bind(&CPDeviceImageTask::doDeviceTask, imageTask, _1);
		DeviceImageParam imageParam = boost::any_cast<DeviceImageParam>(*param);
		getDeviceImageParamStr(imageParam, paramStr, record);
	}
#if (defined OS_PWINDOWS) || (defined OS_PAPPLE)
	else if (taskType == L"task.memoryimage") {
		CPDeviceImageTask imageTask;
		callback = boost::bind(&CPDeviceImageTask::doMemoryTask, imageTask, _1);
		MemoryImageParam imageParam = boost::any_cast<MemoryImageParam>(*param);
		getMemoryImageParamStr(imageParam, paramStr, record);
	}
#endif
	else if (taskType == L"task.initapptree") {
		callback = boost::bind(&CPAnalyzerAssistant::doInitTask, _1);
	}
	else if (taskType == L"task.apps") {
		callback = boost::bind(&CPAnalyzerAssistant::doAppTask, _1);
		getAppParamStr(param, paramStr);
	}
	else if (taskType == L"task.deviceerase") {
		CPDeviceEraseTask eraseTask;
		callback = boost::bind(&CPDeviceEraseTask::doTask, eraseTask, _1);
		DeviceEraseParam eraseParam = boost::any_cast<DeviceEraseParam>(*param);
		getEraseParamStr(eraseParam, paramStr, record);
	}
	else if (taskType == L"task.screenvideo") {
		CPScreenRecordTask screenrecordTask;
		callback = boost::bind(&CPScreenRecordTask::doTask, screenrecordTask, _1);
		VideoParam eraseParam = boost::any_cast<VideoParam>(*param);
		getScreenRecordParamStr(eraseParam, paramStr, record);
	}
	else if (taskType == L"task.fileexport") {
		CPExportFilesTask exportFilesTask;
		callback = boost::bind(&CPExportFilesTask::doTask, exportFilesTask, _1);
		getFileExportParamStr(param, paramStr);
	}
	else {
		assert(false);
	}
}

void getRecordStr(CPRecord* record, std::wstring& strRecord)
{
	CPRecordType* rtype = record->getType();
	const RecordFieldList& fields = rtype->getPublicFields();
	for (auto& field : fields) {
		strRecord += field->display() + L": ";
		std::wstring strValue = CPRecordHandler::variantToString(
			record->at(field->index()), field->decorator(), false);
		strRecord += strValue;
		strRecord += L"\n";
	}
}

void CPSiTaskFactory::getTaskLog(CPTask* task, std::wstring& strLog)
{
	std::wstring taskType = task->type();
	if (taskType == L"task.deviceclone" ||
		taskType == L"task.deviceimage" ||
		taskType == L"task.memoryimage") {
		getRecordStr(task->rootRecord(), strLog);
	}
	else if (taskType == L"task.fileexport") {
		strLog = task->getParamStr();
	}
	else if (taskType == L"task.apps") {
		strLog = task->getParamStr();
	}
}
