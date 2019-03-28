#pragma once

namespace app
{
	class ModulesReliedByProcessHelper
    {
    public:
		struct ModuleInformation
		{
			std::wstring base_addr_;  //Base address of the module in the context of the owning process.
			std::wstring  base_size_; //Size, in bytes, of the module.
			std::wstring version_;
			std::wstring exe_path_;  // Null-terminated string that contains the location, or path, of the module.
		};
		typedef std::vector<ModuleInformation>  ModulesInformation;

		struct  ModulesReliedByOneProcessInformation
		{
			uint32 process_id_;
			std::wstring process_name_;
			std::wstring  cmd_line_;
			ModulesInformation  modules_info_;
		};
		typedef std::vector<ModulesReliedByOneProcessInformation> AllModulesInformation;
		static bool GetAllModules(AllModulesInformation& all_modules_information);

        ModulesReliedByProcessHelper();
        ~ModulesReliedByProcessHelper();
      
		static bool analyze(CPRecord* parentRecord);
    private:
       static  bool GetModulesReliedByProcessInformation(const uint32 process_id,
                ModulesInformation& modules_process_information);
       static std::wstring GetModuleVersion(HMODULE handle_module);
       static WCHAR* GetProceeCmdLine(const uint32 process_id);
	  
    };
}
