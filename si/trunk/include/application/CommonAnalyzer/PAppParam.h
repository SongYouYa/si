#pragma once


namespace app
{
	struct AppParam
	{
		CPRecord* rawRootRecord; //未加载文件系统前，获得的record根节点
		CPRecord* rootRecord;
		bool appInited; //已经加载文件系统并初始化
		AppParam()
			: rawRootRecord(nullptr), rootRecord(nullptr), appInited(false)
		{}
	};
}