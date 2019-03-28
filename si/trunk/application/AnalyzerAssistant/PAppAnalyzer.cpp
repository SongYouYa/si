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

	//递归检查record，如果该app选中，则分析
	void CPAppAnalyzer::Impl::startAppRecursive(PSIHANDLE volume, CPRecord* record,
		CPTaskDispatcher* dispatcher, CPTaskStatusCtrl* statusCtrl)
	{
		RECORD_CHECK_STATE folderCheckState = record->getCheckState();
		if (folderCheckState == RECORD_CHECK_NONE) {
			return;
		}

		//选择分析了该项，则去掉不生成报告标志
		record->removeFlag(RFlagNoReport | RFlagNoShow);

		for (CPRecord* child : record->getChildren()) {
			if (!child->isFolder()) {
				//非目录节点，是分析结果，不是具体app
				break;
			}
			RECORD_CHECK_STATE checkState = child->getCheckState();
			if (checkState == RECORD_CHECK_NONE) {
				continue;
			}
			child->removeFlag(RFlagNoReport | RFlagNoShow);
			//由于操作系统目录节点，保存有该系统所在卷的handle，
			//所以需要找到该节点，把handle传给它的子节点
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
		//分析过的应用不再分析
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
					//成功才设已分析标志，这样失败的，还可以重新分析
					record->setFlags(RFlagAppParsed);
				}
				else {
					std::wstring typeDisplay = record->getType()->display();
					statusCtrl->logInfo(PLOG_ERROR, typeDisplay);
				}
				//把分析节点名称添加子节点条目数，以方便界面查看
				std::wstring newName = record->getType()->display() +
					L"(" + I2WS(record->childrenCount()) + L")";
				record->setNodeName(newName);
				_finishedCount++;
				//进度为完成的分析项数目/总分析项数
				progress->updateProgress(_finishedCount * 1000 / _appCount);
				statusCtrl->progress()->updateSize(_finishedCount);
			};
			dispatcher->dispatch(callbackWrapper, false);
			++_appCount;
		}
		else {
			if (!record->canExpand()) {
				//叶子节点应该要有callback
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

		//利用CPTaskDispatcher类，可以把分析项多线程同时执行
		CPTaskDispatcher dispatcher(3);
		_impl->startAppRecursive(NULL, rootRecord, &dispatcher, statusCtrl);
		CPTaskProgress* taskProgress = statusCtrl->progress();
		taskProgress->setUpdatePeriod(500);

		//通过权比，计算总数
		int curProgress = taskProgress->progress(); //之前文件系统占的进度
		float appProgressRatio = (1000 - curProgress) / 1000.0; //分析占总进度的比例
		_impl->_appCount = _impl->_appCount / appProgressRatio; //通过比列计算出的虚拟总数
		taskProgress->setTotalSize(_impl->_appCount);
		_impl->_finishedCount = _impl->_appCount * curProgress / 1000;

		dispatcher.start();
		dispatcher.join();
	}
}