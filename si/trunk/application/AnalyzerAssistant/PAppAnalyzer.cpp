#include "stdafx.h"
#include "PAppAnalyzer.h"
#include "PCallbackFactory.h"

#include "task/PTaskDispatcher.h"

#include "PReportFlags.h"
#include "plog/PLogContentType.h"

namespace app
{
	class CPAppAnalyzer::Impl
	{
	public:
		Impl()
			: _appCount(0), _finishedCount(0)
		{

		}

		void startAppRecursive(PSIHANDLE volume, CPRecord* record,
			CPTaskDispatcher* dispatcher, CPTaskStatusCtrl* statusCtrl);
		void startAppImpl(PSIHANDLE volume, CPRecord* record,
			CPTaskDispatcher* dispatcher, CPTaskStatusCtrl* statusCtrl);

		std::atomic<int> _appCount;
		std::atomic<int> _finishedCount;
	};

	//�ݹ���record�������appѡ�У������
	void CPAppAnalyzer::Impl::startAppRecursive(PSIHANDLE volume, CPRecord* record,
		CPTaskDispatcher* dispatcher, CPTaskStatusCtrl* statusCtrl)
	{
		RECORD_CHECK_STATE folderCheckState = record->getCheckState();
		if (folderCheckState == RECORD_CHECK_NONE) {
			return;
		}

		//ѡ������˸����ȥ�������ɱ����־
		record->removeFlag(RFlagNoReport | RFlagNoShow);

		for (CPRecord* child : record->getChildren()) {
			if (!child->isFolder()) {
				//��Ŀ¼�ڵ㣬�Ƿ�����������Ǿ���app
				break;
			}
			RECORD_CHECK_STATE checkState = child->getCheckState();
			if (checkState == RECORD_CHECK_NONE) {
				continue;
			}
			child->removeFlag(RFlagNoReport | RFlagNoShow);
			//���ڲ���ϵͳĿ¼�ڵ㣬�����и�ϵͳ���ھ��handle��
			//������Ҫ�ҵ��ýڵ㣬��handle���������ӽڵ�
			std::wstring appName = child->getType()->name();
			if (appName == L"table.systemfolder") {
				PSIHANDLE systemVolume = child->at(L"pvolume").Get<PSIHANDLE>();
				startAppRecursive(systemVolume, child, dispatcher, statusCtrl);
			}
			else {
				if (checkState == RECORD_CHECK_ALL) {
					startAppImpl(volume, child, dispatcher, statusCtrl);
				}
				startAppRecursive(volume, child, dispatcher, statusCtrl);
			}
		}
	}

	void CPAppAnalyzer::Impl::startAppImpl(PSIHANDLE volume, CPRecord* record,
		CPTaskDispatcher* dispatcher, CPTaskStatusCtrl* statusCtrl)
	{
		//��������Ӧ�ò��ٷ���
#ifndef _DEBUG
		if (record->flags() & RFlagAppParsed) {
			return;
		}
#endif

		AppCallback appCallback = CPCallbackFactory::getAppCallback(volume,
			record, statusCtrl);
		if (appCallback) {
			CPTaskProgress* progress = statusCtrl->progress();
			VoidCallback callbackWrapper = [this, statusCtrl, appCallback,
				progress, record]() {
				statusCtrl->doPause();
				if (statusCtrl->isCanceled()) {
					return;
				}
				bool bret = appCallback();
				if (bret) {
					//�ɹ������ѷ�����־������ʧ�ܵģ����������·���
					record->setFlags(RFlagAppParsed);
				}
				else {
					std::wstring typeDisplay = record->getType()->display();
					statusCtrl->logInfo(PLOG_ERROR, typeDisplay);
				}
				//�ѷ����ڵ���������ӽڵ���Ŀ�����Է������鿴
				std::wstring newName = record->getType()->display() +
					L"(" + I2WS(record->childrenCount()) + L")";
				record->setNodeName(newName);
				_finishedCount++;
				//����Ϊ��ɵķ�������Ŀ/�ܷ�������
				progress->updateProgress(_finishedCount * 1000 / _appCount);
				statusCtrl->progress()->updateSize(_finishedCount);
			};
			dispatcher->dispatch(callbackWrapper, false);
			++_appCount;
		}
		else {
			if (!record->canExpand()) {
				//Ҷ�ӽڵ�Ӧ��Ҫ��callback
				assert(false);
			}
		}
	}

	CPAppAnalyzer::CPAppAnalyzer()
		: _impl(nullptr)
	{

	}

	CPAppAnalyzer::~CPAppAnalyzer()
	{
		if (_impl) {
			delete _impl;
			_impl = nullptr;
		}
	}

	void CPAppAnalyzer::doAppTask(CPTask* task, CPRecord* rootRecord)
	{
		_impl = new Impl;

		CPTaskStatusCtrl* statusCtrl = task->statusCtrl();

		//����CPTaskDispatcher�࣬���԰ѷ�������߳�ͬʱִ��
		CPTaskDispatcher dispatcher(3);
		_impl->startAppRecursive(NULL, rootRecord, &dispatcher, statusCtrl);
		CPTaskProgress* taskProgress = statusCtrl->progress();
		taskProgress->setUpdatePeriod(500);

		//ͨ��Ȩ�ȣ���������
		int curProgress = taskProgress->progress(); //֮ǰ�ļ�ϵͳռ�Ľ���
		float appProgressRatio = (1000 - curProgress) / 1000.0; //����ռ�ܽ��ȵı���
		_impl->_appCount = _impl->_appCount / appProgressRatio; //ͨ�����м��������������
		taskProgress->setTotalSize(_impl->_appCount);
		_impl->_finishedCount = _impl->_appCount * curProgress / 1000;

		dispatcher.start();
		dispatcher.join();
	}
}