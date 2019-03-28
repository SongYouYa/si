#pragma once
#include "siutility/globals.h"

namespace siutil
{
	class SI_UTILITY_API CFileSearch
    {
    public:
        //===========================================================
        // brief :		�ڸ�·�����棬����·������������ʽ���ļ���Ŀ¼
        // param		[����]��strBaseDir : Ҫ���ҵ��ļ��ĸ�·��(���Ӿ�Ŀ¼��ʼ��һ��·��, ��ʹ��ͨ�����*��), Ϊ��ʱ����ȫ������
        // param		[����]��strTargetRegex : Ŀ�����·����������ʽ, ���Ϊ�գ��򷵻�baseƥ���handle
        // param		[����]keyNames :     Ŀ������ļ����Ĺؼ���, �������ö��,��"|"��������aa|bb, ���Ϊ���йؼ��ֵĲ���
        // param		[����]��CaseSensitive : �Ƿ����ִ�Сд
        // Returns:	std::wstring : ʵ�ʵ�����·��
        //===========================================================
        CFileSearch(const std::wstring& strBaseDir, const std::wstring& strTargetRegexPath = L"",
                    const std::wstring& keyNames = L"", bool CaseSensitive = false);
        ~CFileSearch();

		//brief : ���Ŀ������Ŀ¼������ָ�����������Ĺؼ���, �ַ����﷨��
		//brief : ��1: "a" OR "b" OR "c"  ��ʾ���������ļ�������a��b��c�е��κ�һ������
		//brief : ��2: "a" AND "b" AND "c" ��ʾ����������ͬʱ�����ļ�������a��b��c����
		//return��bool�� Ŀǰֻ֧��ȫ��ΪOR��ȫ��ΪAND���ַ�������֧��OR��AND���ã��������ʧ�ܣ��򷵻�false
		bool SetSubKeyNames(const std::wstring& subKeyNames);

        //ֻ��handles�������Ӳ���
        void GetHandles(std::vector<PSIHANDLE>& handles);

    private:
		class Impl;
		std::shared_ptr<Impl> impl_;
    };
}
