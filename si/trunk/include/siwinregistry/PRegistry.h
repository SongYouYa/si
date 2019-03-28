#pragma once
#include "PRegistryValue.h"

namespace winreg
{
	class  CPRegistry
	{
	public:
		CPRegistry();
		~CPRegistry();

		//ָ���������Ӽ��������Ӽ�
		//bool bKey64: ��64λϵͳ�У�����64λ�ڵ��µ�λ�ã���wow6432Node��
		static bool OpenKey(const HKEY& rootKey, const std::wstring& subKeyPath, 
			HKEY& subKey, bool bKey64 = false);
		//��ȡָ�����������Ӽ���
		static bool GetSubKeyNames(const HKEY& rootKey, std::vector<std::wstring>& subKeyNames);
		//��ȡָ����������Value����
		static bool GetValueNames(const HKEY&, std::vector<std::wstring>& valueNames);
		//��ȡָ������Value��������
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
		//�ر�ָ����
		static bool getKeyModifyTime(const HKEY& key, PFILETIME pFileTime);
		static bool CloseKey(const HKEY& key);
	};
}
