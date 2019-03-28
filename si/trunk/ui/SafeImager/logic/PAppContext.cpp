#include "stdafx.h"
#include "PAppContext.h"

#include "silicchecker/PSiLicChecker.h"
#include "PLicDefs.h"

#include "version/version2.h"


CPAppContext* CPAppContext::m_instance = nullptr;

CPAppContext* CPAppContext::get()
{
    if (m_instance == nullptr) {
        m_instance = new CPAppContext();
        m_instance->init();
    }
	return m_instance;
}

CPAppContext::CPAppContext()
{

}

std::wstring CPAppContext::getInRootDir()
{
#ifdef OS_PAPPLE
    std::wstring appDir = CPUtils::getAppDir();
    return appDir;
#else
    return CPAppContextBase::getInRootDir();
#endif
}

std::wstring CPAppContext::getOutRootDir()
{
	return L"";
}

std::wstring CPAppContext::getVersionIniPath()
{
	//先获取U盘分区的ini
	std::wstring iniPath = opath(L"conf") + L"/build.ini";
	if (QFile(QString::fromStdWString(iniPath)).exists()) {
		return iniPath;
	}
	iniPath = ipath(L"conf", L"build.ini");
	return iniPath;
}

void CPAppContext::setOutDir(const std::wstring& outDir)
{
    CPAppContextBase::setOutDir(outDir);
    if (!opath(L"").empty()) {
		for(auto& callback : _outCallbacks) {
            callback();
        }
    }
}

void CPAppContext::addOutCallback(const VoidCallback& callback)
{
    _outCallbacks.push_back(callback);
}
