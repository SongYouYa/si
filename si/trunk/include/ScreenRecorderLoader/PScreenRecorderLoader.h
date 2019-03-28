#pragma once
#include "ScreenRecorderLoader/globals.h"

namespace video
{
	class CPScreenRecorderLoader
	{
	public:
		CPScreenRecorderLoader();
		~CPScreenRecorderLoader();
		//设置ScreenRecorder.exe路径
		static void setScreenRecorderPath(const std::wstring& srPath);

		//注册错误回调函数(若没有stop，video进程就结束了，说明出错了)
		void addErrorCallback(const VoidCallback& callback);

		bool start(const std::wstring& videoPath);
		void stop();

	private:
		std::vector<VoidCallback> _callbacks;

		class Impl;
		Impl* _impl;

		static std::wstring _screenRecorderPath;
	};
}
