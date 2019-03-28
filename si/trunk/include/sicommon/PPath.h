#pragma once

class CPPath
{
public:
	//�滻·���зǷ��ַ�
	//isName:�ַ���Ϊ�ļ���
	static std::wstring getValidPath(const std::wstring& path, bool isName);
	//��ȡ���ظ�Ŀ¼���ļ�����������Ŀ¼���ļ��Ѵ��ڣ�������
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
	//��ȡ·�����ڵ�Ŀ¼
	static std::wstring getDir(const std::wstring& path);

	static bool createDirs(const std::wstring& dirs);

	static bool copyFiles(const std::wstring& srcDir, const std::wstring& dstDir);
	//isDstDir: dstPath�Ƿ�Ŀ¼
	static bool copyFile(const std::wstring& srcFile, const std::wstring dstPath, 
		bool isDstDir = false);

	//����·���е�б��:
	//windows��:��/�滻Ϊ\
	//all�� ȥ���ظ�б��
	static void formatPathSlash(std::wstring& path);
};
