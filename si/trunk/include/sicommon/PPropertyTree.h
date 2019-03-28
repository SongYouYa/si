#pragma once

#include <boost/property_tree/ptree.hpp>

class CPPropertyTree
{
public:
	CPPropertyTree();
	~CPPropertyTree();

	//��ȡXml�ļ���ptree��
    static bool LoadXml(const std::wstring& xmlPath, boost::property_tree::wptree& wtree);
    static bool LoadJson(const std::wstring& jsonPath, boost::property_tree::wptree& pt);
};
