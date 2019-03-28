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
        //�����ļ�
        bool SearchFiles();
        //��ȡ������������handle
        void GetSearchedHandles(std::vector<PSIHANDLE>& handles);

	private:
		std::vector<CAppRoot*> GetAppRoots();
		std::wstring GetAppRootPath();
		//����ģ��
		bool ParseApp(std::wstring templatePath);

		std::wstring applicationName_; //����
		std::vector<CAppRoot*> appRoots_; //ģ���µ�Root�㼯
        std::wstring appRootPath_; //����Ŀ¼�ĸ�·��
	};
}
