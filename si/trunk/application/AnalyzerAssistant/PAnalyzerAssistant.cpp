#include "stdafx.h"
#include "application/AnalyzerAssistant/PAnalyzerAssistant.h"
#include "PAppAnalyzer.h"
#include "POsChecker.h"
#include "application/WindowsAnalyzer/PWindowsAnalyzer.h"
#ifdef OS_PUNIX
#include "application/MacAnalyzer/PMacAnalyzer.h"
#endif

#include "PReportFlags.h"
#include "filesystemshell/PFsStatus.h"

namespace app
{
	std::vector<std::wstring> g_hardwareApps = {
		L"table.diskinfo", L"table.partitioninfo"
	};

	void addAppItems(CPRecord* parentFolder, const std::vector<std::wstring>& appTypes)
	{
		CPRecordTypeFactory* typeFactory = CPRecordTypeFactory::get();

		for (auto appName : appTypes) {
			CPRecord* appFolder = typeFactory->getType(appName)->newRecord();
			appFolder->addFlag(RFlagNoReport | RFlagNoShow);
			parentFolder->addChild(appFolder);
		}
	}

	void initHardItems(CPRecord* rootRecord)
	{
		CPRecordTypeFactory* typeFactory = CPRecordTypeFactory::get();
		//硬件信息
		CPRecord* hardwareFolder = typeFactory->getType(L"table.hardwareinfo")->newRecord();
		//初始化时设置不生成报告标志
		hardwareFolder->addFlag(RFlagNoReport | RFlagNoShow);
		rootRecord->addChild(hardwareFolder);
		addAppItems(hardwareFolder, g_hardwareApps);
	}

	void CPAnalyzerAssistant::doInitTask(CPTask* task)
	{
		CPRecord* rootRecord = task->rootRecord();
		initHardItems(rootRecord);

		CPRecordTypeFactory* typeFactory = CPRecordTypeFactory::get();
		//系统信息
		CPRecordType* sysFolderType = typeFactory->getType(L"table.systemfolder");

		//先不加载文件系统，根据运行平台，初始化数据分析项。
		//等开始分析，加载文件系统后，再进一步初始化分析项。
#ifdef OS_PWINDOWS
		CPWindowsAnalyzer::initRawApps(rootRecord);
#elif OS_PLINUX
		CPOsChecker osChecker;
		if (osChecker.hasWinOs()) {
			CPWindowsAnalyzer::initRawApps(rootRecord);
		}
		else if (osChecker.hasMacOs()) {
            CPMacAnalyzer::initRawApps(rootRecord);
		}
#else
		assert(false);
#endif
	}

	//获取选中的分析项类型
	void getCheckedApps(CPRecord* record, std::set<std::wstring>& apps)
	{
		if (record->getCheckState() == RECORD_CHECK_NONE) {
			return;
		}
		if (record->childrenCount() == 0) {
			apps.insert(record->getType()->name());
			return;
		}
		for (auto child : record->getChildren()) {
			getCheckedApps(child, apps);
		}
	}

	void checkAppsByRaw(const std::set<std::wstring>& checkedApps, CPRecord* record)
	{
		//叶子节点app才直接选中
		if (record->childrenCount() == 0) {
			std::wstring appName = record->getType()->name();
			auto it = checkedApps.find(appName);
			if (it != checkedApps.end()) {
				record->setChecked(true);
			}
		}
		else {
			for (auto child : record->getChildren()) {
				checkAppsByRaw(checkedApps, child);
			}
		}
	}

	void waitFs(CPTaskStatusCtrl* statusCtrl)
	{
		if (!CPFsStatus::get()->isLoaded()) {
			//如果文件系统还没有加载好，则这里更新一下文件系统进度
			std::thread t([statusCtrl](){
				CPTaskProgress* taskProgress = statusCtrl->progress();
				taskProgress->setTotalSize(100); //先预设总数为100
				const float fsRatio = 0.2;
				while (true) {
					if (statusCtrl->isCanceled()) {
						break;
					}
					statusCtrl->doPause();
					int progress = CPFsStatus::get()->getProgress() * fsRatio; //文件系统占总进度的20%
					taskProgress->updateProgress(progress);
					taskProgress->updateSize(progress / 1000.0 * 100); //根据进度来计算当前数
					if (CPFsStatus::get()->isLoaded()) {
						break;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
				}
			});
			t.detach();
			//等待文件系统加载完成，再进行分析
			CPFsStatus::get()->waitForLoaded();
		}
	}

	void CPAnalyzerAssistant::doAppTask(CPTask* task)
	{
		AppParam param = boost::any_cast<AppParam>(*task->param());
		CPRecord* rootRecord = param.rootRecord;
		if (!param.appInited) {
			CPTaskStatusCtrl* statusCtrl = task->statusCtrl();
			waitFs(statusCtrl);
			if (statusCtrl->isCanceled()) {
				return;
			}
			initHardItems(rootRecord);
			CPWindowsAnalyzer::initApps(rootRecord);

#ifdef OS_PUNIX
            //现在mac分析比较乱，也没有很好的分卷分析
            CPOsChecker osChecker;
            if (osChecker.hasMacOs()) {
                CPMacAnalyzer::initRawApps(rootRecord);
            }
#endif
			//根据raw节点的选中情况，来选中新初始化的分析项
			std::set<std::wstring> rawCheckedApps;
			getCheckedApps(param.rawRootRecord, rawCheckedApps);
			checkAppsByRaw(rawCheckedApps, rootRecord);
		}

		CPAppAnalyzer analyzer;
		analyzer.doAppTask(task, rootRecord);
	}
}
