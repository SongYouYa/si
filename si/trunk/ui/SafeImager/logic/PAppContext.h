#pragma once

#include "uicommon/context/PAppContextBase.h"

class CPAppContext : public puic::CPAppContextBase
{
private:
	CPAppContext();
public:
	static CPAppContext* get();

    //设置输出目录后的回调
    void addOutCallback(const VoidCallback& callback);
    void setOutDir(const std::wstring& outDir) override;

protected:
    std::wstring getInRootDir() override;
	std::wstring getOutRootDir() override;
	std::wstring getVersionIniPath() override;

private:
    //设置输出目录后的回调
    std::vector<VoidCallback> _outCallbacks;
	static CPAppContext* m_instance;
};
