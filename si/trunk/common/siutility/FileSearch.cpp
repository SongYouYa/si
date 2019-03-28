#include "stdafx.h"
#include "siutility/FileSearch.h"
#include "filesystemshell/FileSystemShell.h"

#include <boost/algorithm/string.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/tuple/tuple.hpp>

using namespace filesystem;
using namespace boost::xpressive;

namespace siutil
{
	const std::wstring LOGIC_AND = L"AND";
	const std::wstring LOGIC_OR  = L"OR";
	const std::wstring F_NAME = L"name"; //此处的名字和配置文件一样
	const std::wstring F_DIR = L"dir";

	std::wstring getNameExpr(const std::wstring& keyName)
	{
		return F_NAME + L" inc " + L"\"" + keyName + L"\"";
	}
	std::wstring getDirExpr()
	{
		return F_DIR + L" > 0";
	}

	class CFileSearch::Impl
	{
	public:
		Impl(const std::wstring& strBaseDir, const std::wstring& strTargetRegexPath = L"",
			const std::wstring& keyNames = L"", bool CaseSensitive = false);
		~Impl();

		bool SetSubKeyNames(const std::wstring& subKeyNames);

		void GetHandles(std::vector<PSIHANDLE>& handles);

	private:
		void SearchTarget();

		bool IsMatchTarget(PSIHANDLE handle);

		void AddTargetHandle(PSIHANDLE targetHandle);

		//通过“KeyNames”、“SubKeyNames”获取子handle
        void GetSubHandlesByKeyNames(PSIHANDLE pRootHandle, std::vector<PSIHANDLE>& subHandles);
		//检测某个handle是否含有设定的子项
		bool CheckHandleBySubKeyNames(PSIHANDLE pItem);
        void GetSubHandlesBySubKeyNames(PSIHANDLE pRootHandle, std::vector<PSIHANDLE>& subHandles);

		bool caseSensitive_; //是否区分大小写
		std::wstring baseDir_; //卷下根路径
		std::wstring targetPath_; //目标路径正则字符串
		boost::xpressive::wsregex targetRegex_; //base目录正则表达式
		std::vector<std::wstring> keyNames_; 
        boost::tuple<std::wstring, std::vector<std::wstring> > tupleSubKeyNames_; //第1个元素: AND、OR；第2个元素:子项关键字
		std::vector<PSIHANDLE> targetHandles_; //匹配到的根路径
	};

	CFileSearch::Impl::Impl( const std::wstring& strBaseDir, const std::wstring& strTargetRegexPath /*= L""*/, 
		const std::wstring& keyNames /*= L""*/, bool CaseSensitive /*= false*/ )
		: baseDir_(strBaseDir), targetPath_(strTargetRegexPath), caseSensitive_(CaseSensitive)
	{
		if(CaseSensitive) {
			targetRegex_ = wsregex::compile(strTargetRegexPath);
		}
		else {
			targetRegex_ = wsregex::compile(strTargetRegexPath, boost::xpressive::icase);
		}

		boost::split(keyNames_, keyNames, boost::is_any_of(L"|"));
		for (auto it = keyNames_.begin(); it != keyNames_.end();) {
			if(it->empty())
				it = keyNames_.erase(it); //删除空字符串
			else
				++it;
		}
	}

	CFileSearch::Impl::~Impl()
	{

	}
    
    bool CFileSearch::Impl::IsMatchTarget(PSIHANDLE handle)
    {
        if(targetPath_.empty()) {
            return true;
        }
        std::wstring path = CFileSystem::GetItemPath(handle);
        return regex_search(path, targetRegex_);
    }

    void CFileSearch::Impl::GetHandles(std::vector<PSIHANDLE>& handles)
    {
        if(targetHandles_.empty()) {
            SearchTarget();
        }
        handles.insert(handles.end(), targetHandles_.begin(), targetHandles_.end());
    }

	bool CFileSearch::Impl::SetSubKeyNames(const std::wstring& subKeyNames)
	{
		if (subKeyNames.empty()) {
			return true;
		}
		
		std::wstring strSubKeyNames = subKeyNames;

		const std::wstring ONE_UNIT_REGEX_STR = L"\\s*[\"]{1}([^\"]*)[\"]{1}\\s*(AND|OR)?(.*)";
		const wsregex unitRegex = wsregex::compile(ONE_UNIT_REGEX_STR, icase);
		while (!strSubKeyNames.empty()) {
			wsmatch what;
			bool bRet = regex_match(strSubKeyNames, what, unitRegex);
			if (!bRet) {
				return false;
			}
			std::vector<std::wstring>& vecSubKeyNames = tupleSubKeyNames_.get<1>();
			std::wstring strkeyname = what[1].str();
			vecSubKeyNames.push_back(strkeyname);

			std::wstring strLogic = what[2].str();
			if (tupleSubKeyNames_.get<0>().empty()) {
				tupleSubKeyNames_.get<0>() = strLogic;
			}
			else if (!strLogic.empty() && !boost::iequals(tupleSubKeyNames_.get<0>(), strLogic)) {
				return false; //字符串中，不能同时有 AND 和 OR 两个逻辑
			}

			strSubKeyNames = what[3].str();
		}

		return true;
	}

    void CFileSearch::Impl::SearchTarget()
    {
		//当“base”、“path”、“keyNames”、“subKeyNames”四个条件都没有设时，不进行搜索
        if(baseDir_.empty() && targetPath_.empty() && keyNames_.empty() && tupleSubKeyNames_.get<1>().empty()) {
            return;
        }

        std::vector<PSIHANDLE> baseHandles;
        if(baseDir_.empty()) {
            CFileSystem::GetAllVolumes(baseHandles);
        }
        else {
            CFileSystem::GetItemHandlesByRawPath(baseDir_, baseHandles, caseSensitive_);
        }
		//当只设置“base”时，返回由“base”匹配到的handle
        if(targetPath_.empty() && keyNames_.empty() && tupleSubKeyNames_.get<1>().empty()) {
            baseHandles.swap(targetHandles_);
            return;
        }

		for (auto it_b = baseHandles.begin(); it_b != baseHandles.end(); ++it_b) {
            PSIHANDLE pRootHandle = *it_b;
            std::vector<PSIHANDLE> subHandles;
			GetSubHandlesByKeyNames(pRootHandle, subHandles);

            for(auto it_s = subHandles.begin(); it_s != subHandles.end(); ++it_s) {
                if(IsMatchTarget(*it_s)) {
                    AddTargetHandle(*it_s);
                }
            }
        }

    }


    void CFileSearch::Impl::AddTargetHandle(PSIHANDLE targetHandle)
    {
        for(auto it = targetHandles_.begin(); it != targetHandles_.end(); ++it) {
            if(targetHandle == *it)
            {
                return;
            }
        }

        targetHandles_.push_back(targetHandle);
    }

    void CFileSearch::Impl::GetSubHandlesByKeyNames( PSIHANDLE pRootHandle, 
		std::vector<PSIHANDLE>& subHandles)
	{
		//如果未设置“KeyNames”、“SubKeyNames”，则获取所有的子handle
		if(keyNames_.empty() && tupleSubKeyNames_.get<1>().empty()) {
			CFileSystem::GetAllSubItems(pRootHandle, subHandles, true);
			return;
		}

		if (!keyNames_.empty()) {
			//通过“KeyNames”获取handle
			std::vector<PSIHANDLE> keyNamesHandles;
			for(auto it_n = keyNames_.begin(); it_n != keyNames_.end(); ++it_n) {
				std::wstring keyName = *it_n;
				std::vector<PSIHANDLE> tempHandles;
				CFileSystem::GetAllSubItems(pRootHandle, tempHandles, true, getNameExpr(keyName));
				keyNamesHandles.insert(keyNamesHandles.begin(), tempHandles.begin(), tempHandles.end());
			}

			for (auto it_key = keyNamesHandles.begin(); it_key != keyNamesHandles.end();) {
				PSIHANDLE pItem = *it_key;
				bool bRet = CheckHandleBySubKeyNames(pItem);
				if (!bRet) {
					it_key = keyNamesHandles.erase(it_key);
				}
				else {
					++it_key;
				}
			}

			subHandles.swap(keyNamesHandles);
		}
		else {
			GetSubHandlesBySubKeyNames(pRootHandle, subHandles);
		}

		return;
	}

    void CFileSearch::Impl::GetSubHandlesBySubKeyNames( PSIHANDLE pRootHandle, std::vector<PSIHANDLE>& subHandles)
	{
		//通过“SubKeyNames”获取handle
		std::vector<PSIHANDLE> dirHandles; //pRootHandle下的目录项
		CFileSystem::GetAllSubItems(pRootHandle, dirHandles, false/*, getDirExpr()*/);
		
		std::vector<PSIHANDLE> subKeyNamesHandles;
		bool bFirstName = true; //是否第一个subKeyName
		for (auto it = tupleSubKeyNames_.get<1>().begin(); it != tupleSubKeyNames_.get<1>().end(); ++it) {
			std::wstring subKeyName = *it;
			std::set<PSIHANDLE> tempSubKeyNameHandles;
			for (auto it_dir = dirHandles.begin(); it_dir != dirHandles.end(); ++it_dir) {
				PSIHANDLE dirHandle = *it_dir;
				std::vector<PSIHANDLE> tempHandles;
				CFileSystem::GetAllSubItems(dirHandle, tempHandles, true, getNameExpr(subKeyName));
				for (auto it_temp = tempHandles.begin(); it_temp != tempHandles.end(); ++it_temp) {
					PSIHANDLE tempParent = CFileSystem::GetItemParent(*it_temp);
					tempSubKeyNameHandles.insert(tempParent);
				}
			}

			if (bFirstName) {
				subKeyNamesHandles.assign(tempSubKeyNameHandles.begin(), tempSubKeyNameHandles.end());
				bFirstName = false;
			}
			else {
				if (boost::iequals(tupleSubKeyNames_.get<0>(), LOGIC_AND)) {
					std::vector<PSIHANDLE> temp_itersectHandles;
					temp_itersectHandles.resize(subKeyNamesHandles.size());
					std::set_intersection(subKeyNamesHandles.begin(), subKeyNamesHandles.end(), 
						tempSubKeyNameHandles.begin(), tempSubKeyNameHandles.end(), temp_itersectHandles.begin());
					temp_itersectHandles.swap(subKeyNamesHandles);
				}
				else if (boost::iequals(tupleSubKeyNames_.get<0>(), LOGIC_OR)) {
					std::vector<PSIHANDLE> temp_unionHandles;
					temp_unionHandles.resize(subKeyNamesHandles.size() + tempSubKeyNameHandles.size());
					std::set_union(subKeyNamesHandles.begin(), subKeyNamesHandles.end(),
						tempSubKeyNameHandles.begin(), tempSubKeyNameHandles.end(), temp_unionHandles.begin());
					temp_unionHandles.swap(subKeyNamesHandles);
				}
			}
		}
		subHandles.insert(subHandles.end(), subKeyNamesHandles.begin(), subKeyNamesHandles.end());
	}

	bool CFileSearch::Impl::CheckHandleBySubKeyNames( PSIHANDLE pItem )
	{
		std::vector<std::wstring> subKeyNames = tupleSubKeyNames_.get<1>();
		if (subKeyNames.empty()) {
			return true;
		}
		int subItemCount = CFileSystem::SubItemCount(pItem);
		for(int i = 0; i < subItemCount; ++i) {
			PSIHANDLE subHandle = CFileSystem::GetSubItemHandle(pItem, i);
			std::wstring subName = CFileSystem::GetItemName(subHandle);
			for (auto it = subKeyNames.begin(); it != subKeyNames.end();) {
				std::wstring keyName = *it;
				bool bRet = boost::ifind_first(subName, keyName);
				if (bRet) {
					//找到一个sub name
					it = subKeyNames.erase(it);
				}
				else {
					++it;
				}
			}
		}
		//找到所有sub name，则返回true
		return subKeyNames.empty();
	}


	CFileSearch::CFileSearch( const std::wstring& strBaseDir, const std::wstring& strTargetRegexPath /*= L""*/, 
		const std::wstring& keyNames /*= L""*/, bool CaseSensitive /*= false*/ )
	{
		impl_ = std::make_shared<Impl>(strBaseDir, strTargetRegexPath, keyNames, CaseSensitive);
	}

	CFileSearch::~CFileSearch()
	{

	}

	bool CFileSearch::SetSubKeyNames( const std::wstring& subKeyNames )
	{
		return impl_->SetSubKeyNames(subKeyNames);
	}

	void CFileSearch::GetHandles(std::vector<PSIHANDLE>& handles)
	{
		impl_->GetHandles(handles);
	}
}
