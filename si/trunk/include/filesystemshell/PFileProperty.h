#pragma once
#include "./globals.h"

namespace filesystem
{
	typedef std::function<void(std::set<PSIHANDLE>&)> ParseCallback;
	class FSS_API CPFileProperty
	{
	public:
		static CPFileProperty* get();

		void parseAppFile();
		//�ǣ�1����0
		//�Ƿ�Ӧ�������ļ�
		int isAppFile(PSIHANDLE handle);
		//�Ƿ�����ļ�
		int isEncrypt(PSIHANDLE handle);

		void setParseAppCallback(const ParseCallback& callback);
		void setParseEncryptCallback(const ParseCallback& callback);

	private:
		CPFileProperty();

		bool _isAppParsed;
		std::set<PSIHANDLE> _appHandles;
		ParseCallback _appCallback;

		bool _isEncParsed;
		std::set<PSIHANDLE> _encHandles;
		ParseCallback _encCallback;

		static CPFileProperty* _instance;
	};
}