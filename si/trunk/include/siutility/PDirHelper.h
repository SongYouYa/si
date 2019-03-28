#pragma once
#include "siutility/globals.h"

namespace siutil
{
	class SI_UTILITY_API CPDirHelper
    {
    public:
        static CPDirHelper *get();

        void setToolDir(const std::wstring &toolpath);
        std::wstring getToolDir();
        void setConfDir(const std::wstring& confDir) {
            _confDir = confDir;
        }
        std::wstring getConfDir() {
            return _confDir;
        }
        void  setCaseRootDir(const std::wstring &caseRootDir);
		//用于放置临时文件
        std::wstring getTempDir();

    private:
        CPDirHelper();
		static CPDirHelper *m_instance;
        std::wstring _toolDir;
        std::wstring _confDir;
		std::wstring _caseRootDir;
    };
}
