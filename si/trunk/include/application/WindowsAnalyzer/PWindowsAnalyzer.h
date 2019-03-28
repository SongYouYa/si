#pragma once
#include "application/WindowsAnalyzer/globals.h"

namespace app
{
	class WIN_APP_API CPWindowsAnalyzer
	{
	public:
		CPWindowsAnalyzer();
		//未加载文件系统前，初始化分析项节点
		static void initRawApps(CPRecord* rootRecord);
		//加载文件系统后，初始化需要分析的应用选项
		static void initApps(CPRecord* rootRecord);

	private:
		//在系统节点下添加windows在线和离线公有的分析项
		static void addCommonApps(CPRecord* sysFolder);
		//在系统节点下添加windows在线分析项
		static void addOnlineApps(CPRecord* sysFolder);
	};
}