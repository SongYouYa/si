#pragma once

namespace app
{
    class  CPMacAnalyzer
	{
	public:
        //未加载文件系统前，初始化分析项节点
        static void initRawApps(CPRecord* rootRecord);
        //加载文件系统后，初始化需要分析的应用选项
        static void initApps(CPRecord* rootRecord);
	private:

	};
}
