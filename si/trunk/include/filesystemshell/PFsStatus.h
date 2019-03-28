#pragma once
#include "./globals.h"

namespace filesystem
{
	class FSS_API CPFsStatus
	{
	public:
		static CPFsStatus* get();

		void setLoaded();
		//�ȴ��ļ�ϵͳ���ؽ���
		void waitForLoaded();
		bool isLoaded();

		//����һ����ŵĽ��ȣ�0-1000
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