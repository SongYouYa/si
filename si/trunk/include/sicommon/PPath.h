#pragma once

class CPPath
{
public:
	//替换路径中非法字符
	//isName:字符串为文件名
	static std::wstring getValidPath(const std::wstring& path, bool isName);
	//获取不重复目录或文件名，若传入目录或文件已存在，则加序号
	static std::wstring getNewPath(const std::wstring& path);

	enum FilterMode
	{
		FILTER_FILE = 1,
		FILTER_DIR = 2
	};
	//ext: ".txt"
	static void getSubNames(const std::wstring& dir, const std::wstring& ext,
		std::vector<std::wstring>& names, 
		int mode, bool recursive = false);
	static std::wstring getFileName(const std::wstring& path);
	//获取路径所在的目录
	static std::wstring getDir(const std::wstring& path);

	static bool createDirs(const std::wstring& dirs);

	static bool copyFiles(const std::wstring& srcDir, const std::wstring& dstDir);
	//isDstDir: dstPath是否目录
	static bool copyFile(const std::wstring& srcFile, const std::wstring dstPath, 
		bool isDstDir = false);

	//处理路径中的斜杠:
	//windows下:把/替换为\
	//all： 去掉重复斜杠
	static void formatPathSlash(std::wstring& path);
};
