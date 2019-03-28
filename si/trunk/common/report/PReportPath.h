#pragma once

namespace report
{
	class CPReportPath
	{
	public:
		//���ñ��������ļ���Ŀ¼
		static void SetConfRootDir(const std::wstring& path);
		//���ö�ý���ļ��ĸ�Ŀ¼
		static void SetMediaRootDir(const std::wstring& dir);

		static std::wstring GetConfRootDir();
		static std::wstring GetHtmlConfDir();

	//utility
	public:
		//�����ļ�
		static bool CopyFiles(const std::wstring& srcDir, const std::wstring& dstDir);
		static bool CopyFile(const std::wstring& srcFile, const std::wstring disFile);

		//��ȡĿ¼�ȼ�
		static uint GetDirLevel(const std::wstring& relativePath);

		//����·����ȥ���ظ��ģ����ģ��ָ�����ʹ֮��Ϊ /a/b/c ����ʽ
		static std::wstring HandlePath(const std::wstring& path);
		//��ȡ����·�������·��
		static std::wstring GetRelativeDir(const std::wstring& path1, const std::wstring& path2);

	private:
		static std::wstring confRootDir_;
		static std::wstring multiMediaRootDir_;
	};
}