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
		//Ӳ����Ϣ
		CPRecord* hardwareFolder = typeFactory->getType(L"table.hardwareinfo")->newRecord();
		//��ʼ��ʱ���ò����ɱ����־
		hardwareFolder->addFlag(RFlagNoReport | RFlagNoShow);
		rootRecord->addChild(hardwareFolder);
		addAppItems(hardwareFolder, g_hardwareApps);
	}

	void CPAnalyzerAssistant::doInitTask(CPTask* task)
	{
		CPRecord* rootRecord = task->rootRecord();
		initHardItems(rootRecord);

		CPRecordTypeFactory* typeFactory = CPRecordTypeFactory::get();
		//ϵͳ��Ϣ
		CPRecordType* sysFolderType = typeFactory->getType(L"table.systemfolder");

		//�Ȳ������ļ�ϵͳ����������ƽ̨����ʼ�����ݷ����
		//�ȿ�ʼ�����������ļ�ϵͳ���ٽ�һ����ʼ�������
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

	//��ȡѡ�еķ���������
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
		//Ҷ�ӽڵ�app��ֱ��ѡ��
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
			//����ļ�ϵͳ��û�м��غã����������һ���ļ�ϵͳ����
			std::thread t([statusCtrl](){
				CPTaskProgress* taskProgress = statusCtrl->progress();
				taskProgress->setTotalSize(100); //��Ԥ������Ϊ100
				const float fsRatio = 0.2;
				while (true) {
					if (statusCtrl->isCanceled()) {
						break;
					}
					statusCtrl->doPause();
					int progress = CPFsStatus::get()->getProgress() * fsRatio; //�ļ�ϵͳռ�ܽ��ȵ�20%
					taskProgress->updateProgress(progress);
					taskProgress->updateSize(progress / 1000.0 * 100); //���ݽ��������㵱ǰ��
					if (CPFsStatus::get()->isLoaded()) {
						break;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
				}
			});
			t.detach();
			//�ȴ��ļ�ϵͳ������ɣ��ٽ��з���
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
            //����mac�����Ƚ��ң�Ҳû�кܺõķ־����
            CPOsChecker osChecker;
            if (osChecker.hasMacOs()) {
                CPMacAnalyzer::initRawApps(rootRecord);
            }
#endif
			//����raw�ڵ��ѡ���������ѡ���³�ʼ���ķ�����
			std::set<std::wstring> rawCheckedApps;
			getCheckedApps(param.rawRootRecord, rawCheckedApps);
			checkAppsByRaw(rawCheckedApps, rootRecord);
		}

		CPAppAnalyzer analyzer;
		analyzer.doAppTask(task, rootRecord);
	}
}
