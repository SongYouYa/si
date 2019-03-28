#include "stdafx.h"
#include "PTaskHandlers.h"
#include "uicommon/task/PTaskCallbackHandler.h"


CPTaskHandlers* CPTaskHandlers::_instance = nullptr;

CPTaskHandlers::CPTaskHandlers(QWidget* parent)
	: _parent(parent)
{

}

CPTaskHandlers* CPTaskHandlers::get(QWidget* parent)
{
	if (!_instance) {
		_instance = new CPTaskHandlers(parent);
	}
	return _instance;
}

void CPTaskHandlers::addTask(CPTask* task)
{
	_callbackHandlers[task] = new CPTaskCallbackHandler(task, _parent);
}

void CPTaskHandlers::delTask(CPTask* task)
{
	if (!_parent) {
		//如果没有object parent，需要手动析构handler
		auto it = _callbackHandlers.find(task);
		if (it != _callbackHandlers.end()) {
			QObject* handler = it->second;
			delete handler;
		}
	}
	_callbackHandlers.erase(task);
}