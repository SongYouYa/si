#include "stdafx.h"
#include "filesystemshell/PFsStatus.h"

#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include <atomic>

namespace filesystem
{
	class CPFsStatus::Impl
	{
	public:
		Impl()
			:_progress(0)
		{}
		boost::mutex _mutex; //加载文件系统锁
		boost::condition_variable_any _condtion;
		std::atomic<int> _progress;
	};

	CPFsStatus* CPFsStatus::_instance = nullptr;

	CPFsStatus::CPFsStatus()
		: _impl(new Impl),
		_loaded(false)
	{

	}
	
	CPFsStatus* CPFsStatus::get()
	{
		if (!_instance) {
			_instance = new CPFsStatus();
		}
		return _instance;
	}

	void CPFsStatus::setLoaded()
	{
		{
			boost::mutex::scoped_lock lock(_impl->_mutex);
			_loaded = true;
		}
		_impl->_condtion.notify_all();
	}

	void CPFsStatus::waitForLoaded()
	{
		boost::mutex::scoped_lock lock(_impl->_mutex);
		if (!_loaded)
		{
			_impl->_condtion.wait(_impl->_mutex);
		}
		return;
	}

	bool CPFsStatus::isLoaded()
	{
		boost::mutex::scoped_lock lock(_impl->_mutex);
		return _loaded;
	}

	void CPFsStatus::setProgress(int progress)
	{
		_impl->_progress = pmin(progress, 1000);
	}

	int CPFsStatus::getProgress()
	{
		return _impl->_progress;
	}
}