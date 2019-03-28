#pragma once
namespace app
{
    class ServiceInfoHelper
    {
    public:
		struct  ServiceInformation
		{
			std::wstring service_name_;
			std::wstring display_name_;
			std::wstring status_;
		};
		typedef std::vector<ServiceInformation>  ServicesInformation;
		static bool analyze(CPRecord* parentRecord);

	private:
        static bool  GetServicesInformation(ServicesInformation& services_information);	
    };
}
