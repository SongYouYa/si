#include "stdafx.h"
#include "siutility/PDirHelper.h"

namespace siutil
{	
	CPDirHelper* CPDirHelper::m_instance = nullptr;
    CPDirHelper::CPDirHelper()
    {
    }

    CPDirHelper * CPDirHelper::get()
    {
		if (m_instance == nullptr){
			m_instance = new CPDirHelper();
        }
		return m_instance;
    }

    std::wstring CPDirHelper::getToolDir()
    {
        return _toolDir;
    }

    void CPDirHelper::setToolDir(const std::wstring &toolpath)
    {
        _toolDir = toolpath;
    }

	void  CPDirHelper::setCaseRootDir(const std::wstring &caseRootDir)
    {
		_caseRootDir = caseRootDir;
    }

	std::wstring  CPDirHelper::getTempDir()
    {
		std::wstring tempPath = _caseRootDir + L"/__temp__";
		try  //调用了异常机制
		{	
			boost::filesystem::path destination_path(tempPath);
			if (!boost::filesystem::exists(destination_path)){
				boost::filesystem::create_directories(destination_path);	
			}
			return tempPath;
		}
		catch (...)   //其他问题进行进入这个，也没有处理进行返回错误
		{
            return L"";
		}
		
    }
}
