#include "stdafx.h"
#include "PReportPath.h"

#include <boost/algorithm/string.hpp>

//css、js等目录名
const std::wstring HTML_CONF_DIR = L"/htmlfiles";

namespace report
{
	std::wstring CPReportPath::confRootDir_;
	std::wstring CPReportPath::multiMediaRootDir_;

	void CPReportPath::SetConfRootDir(const std::wstring& path)
	{
		confRootDir_ = path;
	}

	std::wstring CPReportPath::GetConfRootDir()
	{
		return confRootDir_;
	}

	std::wstring CPReportPath::GetHtmlConfDir()
	{
		return confRootDir_ + HTML_CONF_DIR;
	}

	uint CPReportPath::GetDirLevel(const std::wstring& relativePath)
	{
		std::wstring tempPath = relativePath;
		boost::replace_all(tempPath, L"\\", L"/");
		std::vector<std::wstring> pathTokens;
		boost::split(pathTokens, tempPath, boost::is_any_of(L"/"));

		for (std::vector<std::wstring>::iterator it = pathTokens.begin(); it != pathTokens.end();)
		{
			if (it->empty())
				it = pathTokens.erase(it); //删除空字符串
			else
				++it;
		}

		return pathTokens.size();
	}

	std::wstring CPReportPath::HandlePath(const std::wstring& path)
	{
		if (path.empty())
		{
			return path;
		}

		std::wstring tempPath = path;
		boost::replace_all(tempPath, L"\\", L"/");
		std::vector<std::wstring> pathTokens;
		boost::split(pathTokens, tempPath, boost::is_any_of(L"/"));

		for (std::vector<std::wstring>::iterator it = pathTokens.begin(); it != pathTokens.end();)
		{
			if (it->empty())
				it = pathTokens.erase(it); //删除空字符串
			else
				++it;
		}

		std::wstring newPath;
		if (path.at(0) == L'/') //Linux系统的起始 / 不可去
		{
			newPath = L"/";
		}
		
		for (auto it = pathTokens.begin(); it != pathTokens.end(); ++it)
		{
			newPath.append(*it);
			if (it != pathTokens.end()-1)
			{
				newPath.append(L"/");
			}
		}

		return newPath;
	}

	std::wstring CPReportPath::GetRelativeDir(const std::wstring& path1, const std::wstring& path2)
	{
		std::wstring newPath1 = HandlePath(path1);
		std::wstring newPath2 = HandlePath(path2);

		std::wstring longerPath = (newPath1.size() > newPath2.size()) ? newPath1 : newPath2;
		std::wstring shorterPath = (newPath1.size() > newPath2.size()) ? newPath2 : newPath1;

		std::wstring relativePath;
		auto pos = longerPath.find(shorterPath);
		if (pos != std::string::npos)
		{
			size_t subpos = pos + shorterPath.size() + 1; //加1是因为跳过"/"字符
			if (subpos < longerPath.size())
			{
				relativePath = longerPath.substr(subpos);
			}
		}
		return relativePath;
	}

	void CPReportPath::SetMediaRootDir(const std::wstring& dir)
	{
		multiMediaRootDir_ = dir;
	}

}