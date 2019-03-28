#pragma once

#include <boost/property_tree/ptree.hpp>

class CPPropertyTree
{
public:
	CPPropertyTree();
	~CPPropertyTree();

	//读取Xml文件到ptree中
    static bool LoadXml(const std::wstring& xmlPath, boost::property_tree::wptree& wtree);
    static bool LoadJson(const std::wstring& jsonPath, boost::property_tree::wptree& pt);
};
