#include "stdafx.h"
#include "siutility/PMacFileExport.h"
#include "siutility/MacCommonHelper.h"

namespace siutil
{
    CPMacFileExport::CPMacFileExport(const std::wstring userRawRootPath, const std::wstring fileName)
        : userRawRootPath_(userRawRootPath), fileName_(fileName)
    {
        if(!userRawRootPath.empty() && userRawRootPath.at(0) == L'/')
        {
            userRawRootPath_ = userRawRootPath.substr(1);
        }
    }

    CPMacFileExport::~CPMacFileExport()
    {

    }

    void CPMacFileExport::GetExportFiles(std::vector<std::wstring>& exportFiles)
    {
        if(exportedFiles_.empty())
        {
            std::set<std::wstring> userNames;
            MacCommonHelper::GetMacAllUserName(userNames);

            for(std::set<std::wstring>::iterator it = userNames.begin(); it != userNames.end(); ++it)
            {

                std::vector<std::wstring> tempOutPaths;
                std::wstring rootRawPath = L"/Users/" + *it + L"/" + userRawRootPath_;
                CPFileExportPtr cfxptr(new CPFileExport(rootRawPath, fileName_));
                cfxptr->getExportFiles(tempOutPaths);
                cfxptrs_.push_back(cfxptr);
                exportedFiles_.insert(exportedFiles_.end(), tempOutPaths.begin(), tempOutPaths.end());
            }
        }

        exportFiles = exportedFiles_;
    }


}
