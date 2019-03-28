#pragma once
#include "siregistry/globals.h"
#include "siregistry/PRegFile.h"
#include "PRegistryValue.h"

namespace reg
{
	typedef void* SIPKEY;
	class CPRegistryParser
	{
	public:
		CPRegistryParser();
		//根据reg文件、键路径，打开该键
		static bool openKey(CPRegFile* regFile, const std::wstring& keyPath,
			SIPKEY& key);
		//指定已经打开的key，来打开子key
		static bool openKey(SIPKEY rootKey, const std::wstring& subKeyPath, SIPKEY& key);
		//获取键的指定名称的值
		//valueName可以为空（显示在注册表编辑器中为(默认)）
		static bool getValue(const SIPKEY key, 
			const std::wstring& valueName, CPRegistryValue* pvalue);
		//获取子键名称
		static bool getSubKeyNames(const SIPKEY key,
			std::vector<std::wstring>& subKeyNames);
		//获取键值名称
		static bool getValueNames(const SIPKEY key, std::vector<std::wstring>& valueNames);
		//获取键的最后修改时间
		static bool getKeyModifyTime(const SIPKEY key, PFILETIME pFileTime);
		static void closeKey(const SIPKEY key);
	};
}
