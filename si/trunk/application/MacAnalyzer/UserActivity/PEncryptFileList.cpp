#include "stdafx.h"
#include "FileAnalyzer/PEncryptFile.h"
#include "siutility/PFileOperate.h"
#include "PEncryptFileList.h"
using namespace pfile;
namespace macbasic
{
    CPEncryptFileList::CPEncryptFileList()
    {

    }

    bool CPEncryptFileList::getEncrypTFileList(CPRecord *rootRecord )
    {

        std::set<PSIHANDLE> handles;
        CPEncryptFile::getAllEncryptHandles(handles);
        for(std::set<PSIHANDLE>::iterator it_i = handles.begin(); it_i != handles.end(); ++it_i)
            {
                PSIHANDLE handle = *it_i;
                FileItemInfo fileInfo;
                CPFileOperate::ParseFileInfo(handle, fileInfo, true); //此处解析lnk实际路径
                fileInfo.AddToTable(rootRecord);
            }

        return true;
    }
}
