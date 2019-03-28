#pragma once

namespace tpfile
{
	class CAppTarget;
	class CPathRegex;
	class CTemplateApp;
	//RootΪӦ�ó����һ����ȡ��·��
	class CAppRoot
	{
	public:
		CAppRoot();
		~CAppRoot();

		// ���ø�·������
        void SetRootPath(const std::wstring& rootPath);
        std::wstring RootPath();
		// ����baseĿ¼
		void SetBaseDir(const std::wstring& baseDir);
        std::wstring BaseDir();
		void SetOsType(const std::wstring& osType);
		void SetVersion(const std::wstring& version);
		void SetName(const std::wstring& name);
		void SetKeyNames(const std::wstring& keyNames); //���ò��Ҹ�Ŀ¼ʱ����Ŀ¼�Ĺؼ�·��
		void SetSubKeyNames(const std::wstring& subKeyNames);
		std::wstring OsType();
		std::wstring Version();
		std::wstring Name();

		void SetParent(CTemplateApp* parent);
		CTemplateApp* GetParent();

		//�ӽڵ�
		void AddAppTarget(CAppTarget* pAppTarget);
		size_t AppTargetCount(); 

		//�����ļ�
        void SearchFiles();
		//��ȡ���������ļ�
		uint64 SearchedFilesCount();
		//��ȡ������������handle
		void GetSearchedHandles(std::vector<PSIHANDLE>& handles);
        //��ȡ�ѵ����ļ���С
        uint64 GetExportedSize();
        // ���ɵ����ļ����嵥�ļ�
        void CreateExportFileBill(PSIHANDLE pRootItem, const std::wstring& billFilePath);

	private:
		//������·��
        void SearchRoot();
		//�ڸ�·���£���ȡĿ���ļ�handle
		void PickFile();
		bool PickTargetFile(CAppTarget*, PSIHANDLE rootItem);
		bool PickPathRegexFile(CPathRegex* pPathRegex, PSIHANDLE subRootItem);

		std::wstring name_; //root��
		std::wstring osType_;       //����ϵͳ����
		std::wstring version_;      //Ӧ�õİ汾��

        std::wstring baseDir_; //��Ŀ¼
        std::wstring rootPath_; //root·��������ʽ�ַ���
		std::wstring keyNames_;
		std::wstring subKeyNames_;


		boost::xpressive::wsregex rootRegex_; //rootĿ¼������ʽ
		std::vector<PSIHANDLE> realRootPath_; //ƥ�䵽�ĸ�·��
		std::vector<CAppTarget*> appTargets_; //ģ���е�Target
		CTemplateApp* parent_;
	};

}