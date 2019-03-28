#include "stdafx.h"
#include "PAppListAnalyzer.h"
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QDateTime>
#include "siutility/PFileExport.h"

namespace macbasic {

std::map<std::wstring, InstallInfo> CPAppListAnalyzer::m_mapInstall;

CPAppListAnalyzer::CPAppListAnalyzer()
{
}

CPAppListAnalyzer::~CPAppListAnalyzer()
{
    try {
        Clear();
    }
    catch (...) {
    }
}

const OSXApp *CPAppListAnalyzer::FirstChild()
{
    if (m_vectorApp.empty()) {
        return NULL;
    }
    m_itCurrent = m_vectorApp.begin();
    return *m_itCurrent;
}


const OSXApp *CPAppListAnalyzer::NextChild()
{
    if (m_vectorApp.empty()) {
        return NULL;
    }
    ++m_itCurrent;
    if (m_vectorApp.end() == m_itCurrent){
        return NULL;
    }
    return *m_itCurrent;
}

bool CPAppListAnalyzer::AddApp(PSIHANDLE appDirHandle)
{
    std::wstring appDirPath = CFileSystem::GetItemPath(appDirHandle);
    size_t posBegin = appDirPath.rfind(L"/");
    size_t posEnd = appDirPath.rfind(L".app");
    if (std::wstring::npos == posBegin || std::wstring::npos == posEnd
            || posBegin >= posEnd) {
        return false;
    }
    std::wstring wstrBaseName = appDirPath.substr(posBegin + 1, posEnd - posBegin - 1);
    if(wstrBaseName.empty())
    {
        return false;
    }
    OSXApp * pApp = new OSXApp;
    pApp->wstrName = wstrBaseName;

    std::wstring rawPath = L"/Contents/MacOS/" + wstrBaseName;
    PSIHANDLE pItemHandle = CFileSystem::GetItemHandleByRawPath(appDirHandle, rawPath, true);
    if(NULL == pItemHandle)
    {
        return false;
    }

    pApp->lastReadTime = CFileSystem::GetItemLastAccessTime(pItemHandle);
    pApp->modifyTime = CFileSystem::GetItemModfiyTime(pItemHandle);
    pApp->lastStatusTime = CFileSystem::GetItemCreateTime(pItemHandle);
#ifdef _DEBUG
    std::wstring itempath = CFileSystem::GetItemPath(pItemHandle);
    QFileInfo infoFile(QString::fromStdWString(itempath));
    time_t lastReadTime = infoFile.lastRead().toTime_t();
    time64_t l2 = pApp->lastReadTime.ToTime64_t();
    if(lastReadTime != l2)
    {
        std::cout << "file time diff : " << l2 - lastReadTime << infoFile.absoluteFilePath().toStdString() << std::endl;
    }
#endif
    PSIHANDLE pParent = CFileSystem::GetItemParent(appDirHandle);
    pApp->wstrPath = CFileSystem::GetItemPath(pParent);

    if (std::wstring::npos != appDirPath.find(L"/Utilities/")) {
        pApp->installSource = SystemInternal;
    }
    else {
        MapInstallInfo::iterator it_m = m_mapInstall.find(wstrBaseName);
        if(it_m == m_mapInstall.end())
        {
            pApp->installSource = NormalInstall;
        }
        else
        {
            pApp->installSource = it_m->second.installSource;
        }
    }

    std::wstring plistRawPath = L"Contents/Info.plist";
    PSIHANDLE pPlistHandle = CFileSystem::GetItemHandleByRawPath(appDirHandle, plistRawPath, true);
    CPHandleExport fileExport;
    std::wstring plistFilePath =  fileExport.Export(pPlistHandle);
    PlistAnalyzer(plistFilePath, pApp);

    m_vectorApp.push_back(pApp);
    return true;
}


bool CPAppListAnalyzer::PlistAnalyzer(const std::wstring &wstrFile, OSXApp * pApp)
{
    std::string szPath = CPCodeChanger::UnicodeToUTF8(wstrFile.c_str());

    try {
        plistcpp::dictionary_type dict;
        plistcpp::CPlistCpp::ReadPlist(szPath.c_str(), dict);

        // Version
        plistcpp::dictionary_type::iterator it = dict.find("CFBundleShortVersionString");
        if (dict.end() == it) {
            return false;
        }
        std::string strVersion = boost::any_cast<std::string>(it->second);

        // Info String
        it = dict.find("CFBundleGetInfoString");
        if (dict.end() == it) {
            return false;
        }
        std::string strInfo = boost::any_cast<std::string>(it->second);

        pApp->wstrVersion = QString::fromStdString(strVersion).toStdWString();
        pApp->wstrInfoString = QString::fromStdString(strInfo).toStdWString();
    }
    catch (...) {
#ifdef _DEBUG
        std::cout << "app:info parse error: " << szPath << "" << std::endl;
#endif
        return false;
    }
    return true;
}


bool CPAppListAnalyzer::ParseInstallHistory(const std::wstring &wstrFile)
{
    m_mapInstall.clear();

    if (wstrFile.empty()) {
        return false;
    }

    std::string szPath = CPCodeChanger::UnicodeToUTF8(wstrFile.c_str());

    try {
        plistcpp::array_type array;
        plistcpp::CPlistCpp::ReadPlist(szPath.c_str(), array);
        for (plistcpp::array_type::iterator itArray = array.begin();
             array.end() != itArray; ++itArray) {
            plistcpp::dictionary_type dict = boost::any_cast<plistcpp::dictionary_type>(*itArray);
            // app name
            plistcpp::dictionary_type::iterator itDict = dict.find("displayName");
            if (dict.end() == itDict) {
                continue;
            }
            std::string strText = boost::any_cast<std::string>(itDict->second);
            std::wstring wstrName = CPCodeChanger::UTF8ToUnicode(strText.c_str());
#ifdef _DEBUG
        std::cout << "app:installHistory:name: " << strText << "" << std::endl;
#endif

            InstallInfo info;
            // install
            itDict = dict.find("processName");
            if (dict.end() != itDict) {
                strText = boost::any_cast<std::string>(itDict->second);
                if (strText == "storedownloadd" || strText == "storeagent") {
                    info.installSource = AppStoreInstall;
                }
                else {
                    info.installSource = NormalInstall;
                }
            }

            // version
            itDict = dict.find("displayVersion");
            if (dict.end() != itDict) {
                strText = boost::any_cast<std::string>(itDict->second);
                info.wstrVersion = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }

            m_mapInstall[wstrName] = info;
        }
    }
    catch (...) {
#ifdef _DEBUG
        std::cout << "app:installHistory parse error: " << szPath << "" << std::endl;
#endif
        return false;
    }
    return true;
}

void CPAppListAnalyzer::Clear()
{
    if (!m_vectorApp.empty()) {
        for (std::vector<OSXApp *>::iterator it = m_vectorApp.begin();
             it != m_vectorApp.end(); ++it) {
            if (NULL == *it) {
                continue;
            }
            delete *it;
            *it = NULL;
        }
        m_vectorApp.clear();
    }

    m_vectorPlist.clear();
}

}
