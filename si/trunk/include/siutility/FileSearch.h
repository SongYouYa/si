#pragma once
#include "siutility/globals.h"

namespace siutil
{
	class SI_UTILITY_API CFileSearch
    {
    public:
        //===========================================================
        // brief :		在根路径下面，查找路径符合正则表达式的文件或目录
        // param		[输入]：strBaseDir : 要查找的文件的根路径(即从卷目录开始的一段路径, 可使用通配符‘*’), 为空时，则全盘搜索
        // param		[输入]：strTargetRegex : 目标项的路径的正则表达式, 如果为空，则返回base匹配的handle
        // param		[输入]keyNames :     目标项的文件名的关键字, 可以设置多个,用"|"隔开，如aa|bb, 结果为所有关键字的并集
        // param		[输入]：CaseSensitive : 是否区分大小写
        // Returns:	std::wstring : 实际导出的路径
        //===========================================================
        CFileSearch(const std::wstring& strBaseDir, const std::wstring& strTargetRegexPath = L"",
                    const std::wstring& keyNames = L"", bool CaseSensitive = false);
        ~CFileSearch();

		//brief : 如果目标项是目录，这里指定其子项名的关键字, 字符串语法：
		//brief : 例1: "a" OR "b" OR "c"  表示子项中有文件名包含a、b、c中的任何一个即可
		//brief : 例2: "a" AND "b" AND "c" 表示子项中需中同时包含文件名包含a、b、c的项
		//return：bool： 目前只支持全部为OR或全部为AND的字符串，不支持OR、AND混用，如果解析失败，则返回false
		bool SetSubKeyNames(const std::wstring& subKeyNames);

        //只对handles进行增加操作
        void GetHandles(std::vector<PSIHANDLE>& handles);

    private:
		class Impl;
		std::shared_ptr<Impl> impl_;
    };
}
