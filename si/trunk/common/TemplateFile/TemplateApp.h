#pragma once

namespace tpfile
{
	class CAppRoot;
	class CTemplateApp
	{
	public:
		CTemplateApp();
        CTemplateApp(CTemplateApp& rh);
		~CTemplateApp();

        bool ParseTemplate(const std::wstring& templatePath);
        std::wstring GetApplicationName();
        //搜索文件
        bool SearchFiles();
        //获取搜索到的所有handle
        void GetSearchedHandles(std::vector<PSIHANDLE>& handles);

	private:
		std::vector<CAppRoot*> GetAppRoots();
		std::wstring GetAppRootPath();
		//解析模板
		bool ParseApp(std::wstring templatePath);

		std::wstring applicationName_; //名称
		std::vector<CAppRoot*> appRoots_; //模板下的Root层集
        std::wstring appRootPath_; //导出目录的根路径
	};
}
