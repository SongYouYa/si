#pragma once
#include "ScreenRecorderLoader/globals.h"

namespace video
{
	class CPScreenRecorderLoader
	{
	public:
		CPScreenRecorderLoader();
		~CPScreenRecorderLoader();
		//����ScreenRecorder.exe·��
		static void setScreenRecorderPath(const std::wstring& srPath);

		//ע�����ص�����(��û��stop��video���̾ͽ����ˣ�˵��������)
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
