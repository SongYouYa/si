#pragma once
#include "siutility/PFileOperate.h"
namespace macbasic {
class CPlogListAnalyzer
{
public:

    CPlogListAnalyzer();

    bool getFileListInfos(CPRecord *rootRecord);

    bool analyzeFileInfo(CPRecord *rootRecord,const std::wstring &xmlpath);
    ~CPlogListAnalyzer();


};
}
