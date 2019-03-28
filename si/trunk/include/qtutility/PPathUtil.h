#pragma once


namespace qtutil
{
	class CPPathUtil
	{
	public:
		//打开指定文件的资源管理器
		static void openExplorer(const std::wstring& filePath);
	};
}