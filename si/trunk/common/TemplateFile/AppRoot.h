#pragma once

namespace tpfile
{
	class CAppTarget;
	class CPathRegex;
	class CTemplateApp;
	//Root为应用程序的一个提取根路径
	class CAppRoot
	{
	public:
		CAppRoot();
		~CAppRoot();

		// 设置根路径正则
        void SetRootPath(const std::wstring& rootPath);
        std::wstring RootPath();
		// 设置base目录
		void SetBaseDir(const std::wstring& baseDir);
        std::wstring BaseDir();
		void SetOsType(const std::wstring& osType);
		void SetVersion(const std::wstring& version);
		void SetName(const std::wstring& name);
		void SetKeyNames(const std::wstring& keyNames); //设置查找根目录时，根目录的关键路径
		void SetSubKeyNames(const std::wstring& subKeyNames);
		std::wstring OsType();
		std::wstring Version();
		std::wstring Name();

		void SetParent(CTemplateApp* parent);
		CTemplateApp* GetParent();

		//子节点
		void AddAppTarget(CAppTarget* pAppTarget);
		size_t AppTargetCount(); 

		//搜索文件
        void SearchFiles();
		//获取搜索到的文件
		uint64 SearchedFilesCount();
		//获取搜索到的所有handle
		void GetSearchedHandles(std::vector<PSIHANDLE>& handles);
        //获取已导出文件大小
        uint64 GetExportedSize();
        // 生成导出文件的清单文件
        void CreateExportFileBill(PSIHANDLE pRootItem, const std::wstring& billFilePath);

	private:
		//搜索根路径
        void SearchRoot();
		//在根路径下，获取目标文件handle
		void PickFile();
		bool PickTargetFile(CAppTarget*, PSIHANDLE rootItem);
		bool PickPathRegexFile(CPathRegex* pPathRegex, PSIHANDLE subRootItem);

		std::wstring name_; //root名
		std::wstring osType_;       //操作系统类型
		std::wstring version_;      //应用的版本号

        std::wstring baseDir_; //根目录
        std::wstring rootPath_; //root路径正则表达式字符串
		std::wstring keyNames_;
		std::wstring subKeyNames_;


		boost::xpressive::wsregex rootRegex_; //root目录正则表达式
		std::vector<PSIHANDLE> realRootPath_; //匹配到的根路径
		std::vector<CAppTarget*> appTargets_; //模板中的Target
		CTemplateApp* parent_;
	};

}