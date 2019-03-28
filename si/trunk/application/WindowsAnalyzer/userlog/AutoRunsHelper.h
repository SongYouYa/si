#pragma once
namespace app
{

	class AutoRunsHelper //:public MemPipeParser
	{
	public:
		typedef std::vector<std::wstring> AutoRunsInformation;
		typedef	std::vector<AutoRunsInformation> vecAutoRunsInformation;

		enum AutoData
		{
			ENTRYLOCATION = 0,
			ENTRY,
			ENABLED,
			DESCRIPTION,
			PUBLISHER,
			IMAGEPATH,
			LAUNCHSTRING,
			MDSTRING,
			SHA1STRING,
			SHA256STRING
		};

		static bool analyze(CPRecord *rootType);
		AutoRunsHelper(void);
		~AutoRunsHelper(void);
	private:
		static bool GetAutoRunsInformation(vecAutoRunsInformation &vecAutoruns);
	};
}
