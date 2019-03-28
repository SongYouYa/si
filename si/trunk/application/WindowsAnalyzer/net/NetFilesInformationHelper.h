#pragma once
namespace app
{
    class NetFilesInformationHelper
    {
    public:
		struct  NetShareSourceInformation
		{
			std::wstring net_name_;
			std::wstring path_;
			std::wstring current_uses_;
			std::wstring security_descriptor_;
			std::wstring password_;
		};
		typedef std::vector<NetShareSourceInformation> NetShareSourcesInformation;
		struct NetFileInformation
		{
			std::wstring file_name_;
			std::wstring user_name_;
			uint32 lockes_;
			std::wstring access_attribute_;
		};
		typedef std::vector<NetFileInformation> NetFilesInformation;
		static bool analyze(CPRecord* rootRecord);
	
	private:
       static bool  GetNetFilesInformation(NetFilesInformation& net_files_information);
	   static void EnumServer();
	  
    };
}
