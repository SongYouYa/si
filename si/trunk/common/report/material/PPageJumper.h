#pragma once

namespace report
{
	class CPPage;
	class CPPageJumper
	{
	public:
        CPPageJumper();
		~CPPageJumper();

		static void getIndexFilenames(size_t recordCount, size_t unitCount, 
			std::map<uint, std::wstring>& index_filenames);

		static void setPageJump(TemplateDictionary* pageJumpDict, 
			const std::map<uint, std::wstring>& index_FileNames,
			int currentIndex);

		
	private:
		static void getBesidePages(const std::map<uint, std::wstring>& index_FileNames, int currentIndex,
			std::vector<std::pair<int, std::wstring>>& beside_index_FileNames, 
			std::wstring& previseHtmlFile, std::wstring& laterHtmlFile);

		static TemplateDictionary* createPageArchorDict(TemplateDictionary* pPageJumpDict, const std::string& archorText);
		static void setPageArchorHref(ctemplate::TemplateDictionary* pPageArchorDict, const std::string& strHref);
		static void setPageArchorClass(ctemplate::TemplateDictionary* pPageArchorDict, const std::string& strClass);

	};
}
