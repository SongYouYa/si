#pragma once

namespace report
{
	class CPReportPath
	{
	public:
		//设置报告配置文件主目录
		static void SetConfRootDir(const std::wstring& path);
		//设置多媒体文件的根目录
		static void SetMediaRootDir(const std::wstring& dir);

		static std::wstring GetConfRootDir();
		static std::wstring GetHtmlConfDir();

	//utility
	public:
		//拷贝文件
		static bool CopyFiles(const std::wstring& srcDir, const std::wstring& dstDir);
		static bool CopyFile(const std::wstring& srcFile, const std::wstring disFile);

		//获取目录等级
		static uint GetDirLevel(const std::wstring& relativePath);

		//处理路径，去掉重复的（最后的）分隔符，使之成为 /a/b/c 的样式
		static std::wstring HandlePath(const std::wstring& path);
		//获取两个路径的相对路径
		static std::wstring GetRelativeDir(const std::wstring& path1, const std::wstring& path2);

	private:
		static std::wstring confRootDir_;
		static std::wstring multiMediaRootDir_;
	};
}