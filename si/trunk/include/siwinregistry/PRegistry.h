#pragma once
#include "PRegistryValue.h"

namespace winreg
{
	class  CPRegistry
	{
	public:
		CPRegistry();
		~CPRegistry();

		//指定根键、子键名，打开子键
		//bool bKey64: 在64位系统中，访问64位节点下的位置（非wow6432Node）
		static bool OpenKey(const HKEY& rootKey, const std::wstring& subKeyPath, 
			HKEY& subKey, bool bKey64 = false);
		//获取指定键的所有子键名
		static bool GetSubKeyNames(const HKEY& rootKey, std::vector<std::wstring>& subKeyNames);
		//获取指定键的所有Value名称
		static bool GetValueNames(const HKEY&, std::vector<std::wstring>& valueNames);
		//读取指定键、Value名的数据
		template<typename T>
		static bool ReadValueData(const HKEY& key, const std::wstring& valueName, std::vector<T>& valueData)
		{
			DWORD dwSize = 0;
			DWORD dwType = REG_NONE;

			if (RegQueryValueExW(key,
				valueName.c_str(),
				NULL,
				&dwType,
				NULL,
				&dwSize) != ERROR_SUCCESS)
			{
				return false;
			}

			valueData.resize(dwSize / sizeof(T));

			if (RegQueryValueExW(key,
				valueName.c_str(),
				NULL,
				&dwType,
				(LPBYTE)valueData.data(),
				&dwSize) != ERROR_SUCCESS)
			{
				return false;
			}
			return true;
		}
		static bool getValue(const HKEY& key, const std::wstring& valueName, CPRegistryValue* pvalue);
		//关闭指定键
		static bool getKeyModifyTime(const HKEY& key, PFILETIME pFileTime);
		static bool CloseKey(const HKEY& key);
	};
}
