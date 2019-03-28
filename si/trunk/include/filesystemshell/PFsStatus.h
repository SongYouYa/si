#pragma once
#include "./globals.h"

namespace filesystem
{
	class FSS_API CPFsStatus
	{
	public:
		static CPFsStatus* get();

		void setLoaded();
		//等待文件系统加载结束
		void waitForLoaded();
		bool isLoaded();

		//设置一个大概的进度，0-1000
		void setProgress(int progress);
		int getProgress();

	private:
		CPFsStatus();

		class Impl;
		Impl* _impl;
		bool _loaded;

		static CPFsStatus* _instance;
	};
}