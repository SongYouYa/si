#pragma once
#include "application/WindowsAnalyzer/globals.h"

namespace app
{
	class WIN_APP_API CPWindowsAnalyzer
	{
	public:
		CPWindowsAnalyzer();
		//δ�����ļ�ϵͳǰ����ʼ��������ڵ�
		static void initRawApps(CPRecord* rootRecord);
		//�����ļ�ϵͳ�󣬳�ʼ����Ҫ������Ӧ��ѡ��
		static void initApps(CPRecord* rootRecord);

	private:
		//��ϵͳ�ڵ������windows���ߺ����߹��еķ�����
		static void addCommonApps(CPRecord* sysFolder);
		//��ϵͳ�ڵ������windows���߷�����
		static void addOnlineApps(CPRecord* sysFolder);
	};
}