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
		//����reg�ļ�����·�����򿪸ü�
		static bool openKey(CPRegFile* regFile, const std::wstring& keyPath,
			SIPKEY& key);
		//ָ���Ѿ��򿪵�key��������key
		static bool openKey(SIPKEY rootKey, const std::wstring& subKeyPath, SIPKEY& key);
		//��ȡ����ָ�����Ƶ�ֵ
		//valueName����Ϊ�գ���ʾ��ע���༭����Ϊ(Ĭ��)��
		static bool getValue(const SIPKEY key, 
			const std::wstring& valueName, CPRegistryValue* pvalue);
		//��ȡ�Ӽ�����
		static bool getSubKeyNames(const SIPKEY key,
			std::vector<std::wstring>& subKeyNames);
		//��ȡ��ֵ����
		static bool getValueNames(const SIPKEY key, std::vector<std::wstring>& valueNames);
		//��ȡ��������޸�ʱ��
		static bool getKeyModifyTime(const SIPKEY key, PFILETIME pFileTime);
		static void closeKey(const SIPKEY key);
	};
}
