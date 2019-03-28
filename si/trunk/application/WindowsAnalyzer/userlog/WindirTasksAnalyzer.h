#pragma once
namespace app
{
	struct WindirTaskInfo
	{
		std::wstring name_;
		CPTime createTime_;
		CPTime accessTime_;
		CPTime modifyTime_;
	};
	typedef std::vector<WindirTaskInfo> WindirTaskInfos;

	class CWindirTaskAnalyzer
	{
	public:
		CWindirTaskAnalyzer();
		~CWindirTaskAnalyzer();

		static bool analyze(CPRecord *rootRecord);

	private:
		static bool Parse(WindirTaskInfos& infos);
	};
}
