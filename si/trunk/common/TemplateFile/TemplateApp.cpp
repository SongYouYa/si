#include "stdafx.h"
#include "TemplateApp.h"
#include "AppRoot.h"
#include "AppTarget.h"
#include "PathRegex.h"

namespace tpfile
{
	//解析一个Root
	bool ParseRoot(QDomElement* pRootElem, CAppRoot* pAppRoot);
	//解析每个target
	bool ParseTarget(QDomElement* pTargetElem, CAppTarget* pAppTarget);
	//解析每个regexp path
	bool ParsePathRegex(QDomElement* pPathRegexElem, CPathRegex* pPathRegex);

	CTemplateApp::CTemplateApp()
	{
	}

	CTemplateApp::~CTemplateApp()
	{
		std::vector<CAppRoot*>::iterator it = appRoots_.begin();
		for(; it != appRoots_.end(); ++it)
		{
			if(*it)
			{
				delete *it;
			}
		}
	}

    CTemplateApp::CTemplateApp(CTemplateApp& rh)
    {
        applicationName_ = rh.GetApplicationName();
        appRoots_ = rh.GetAppRoots();
        appRootPath_ = rh.GetAppRootPath();
    }

    std::vector<CAppRoot*> CTemplateApp::GetAppRoots()
    {
        return appRoots_;
    }
    std::wstring CTemplateApp::GetAppRootPath()
    {
        return appRootPath_;
    }

	bool CTemplateApp::ParseApp(std::wstring templatePath)
	{
		//1.load
		QDomDocument doc;
		QFile xmlfile(QString::fromStdWString(templatePath));
		if (!xmlfile.open(QIODevice::ReadOnly))
		{
			return false; //open failed
		}

		QString errMsg;
		int errLine;
		int errColumn;

		if (!doc.setContent(&xmlfile,false,&errMsg,&errLine,&errColumn))
		{
			xmlfile.close();
			return false;
		}

		//2.parse app
		QDomElement appElem = doc.documentElement();
		if (appElem.tagName() != "Application")
		{
			xmlfile.close();
			return false;
		}
		applicationName_ = appElem.attribute("name").toStdWString();

		//3. parse root
		QDomNodeList rootList = appElem.childNodes();
		for (int i = 0; i < rootList.size(); i++)
		{
			QDomElement rootElem = rootList.at(i).toElement();
			CAppRoot* pRoot = new CAppRoot();
			if(!ParseRoot(&rootElem, pRoot))
			{
				delete pRoot;
				continue;
			}
			pRoot->SetParent(this);
			appRoots_.push_back(pRoot);
		}
		xmlfile.close();
		return appRoots_.size() > 0;
	}

	bool ParseRoot( QDomElement* pRootElem, CAppRoot* pAppRoot )
	{
		if(!pRootElem->isElement() || pRootElem->tagName() != "root")
		{
			return false;
		}

		//parse root attribute
		std::wstring name = pRootElem->attribute("name").toStdWString();
		pAppRoot->SetName(name);
		std::wstring baseDir = pRootElem->attribute("base").toStdWString();
        pAppRoot->SetBaseDir(baseDir);
		std::wstring rootPath = pRootElem->attribute("path").toStdWString();
        pAppRoot->SetRootPath(rootPath);
		std::wstring keyNames = pRootElem->attribute("keyNames").toStdWString();
		pAppRoot->SetKeyNames(keyNames);
		std::wstring subKeyNames = pRootElem->attribute("SubKeyNames").toStdWString();
		pAppRoot->SetSubKeyNames(subKeyNames);
		std::wstring osType = pRootElem->attribute("OS").toStdWString();
		pAppRoot->SetOsType(osType);
		std::wstring version = pRootElem->attribute("version").toStdWString();
		pAppRoot->SetVersion(version);

		QDomNodeList targetList = pRootElem->childNodes();
		for(int i = 0; i < targetList.size(); ++i)
		{
			QDomElement targetElem = targetList.at(i).toElement();
			CAppTarget* pAppTarget = new CAppTarget;
			if(!ParseTarget(&targetElem, pAppTarget))
			{
				delete pAppTarget;
				continue;
			}
			pAppTarget->SetParent(pAppRoot);
			pAppRoot->AddAppTarget(pAppTarget);
		}
		return pAppRoot->AppTargetCount() > 0;
	}

	bool ParseTarget( QDomElement* pTargetElem, CAppTarget* pAppTarget )
	{
		if (!pTargetElem->isElement() || pTargetElem->tagName() != "target")
		{
			return false;
		}

		//parse target attribute
		std::wstring targetName = pTargetElem->attribute("name").toStdWString();
		pAppTarget->SetName(targetName);

		//parse pathRegex node
		QDomNodeList pathRegexList = pTargetElem->childNodes();
		for (int i = 0; i < pathRegexList.size(); i++)
		{
			QDomElement regexPathElem = pathRegexList.at(i).toElement();
			CPathRegex* pPathRegex = new CPathRegex;
			pPathRegex->SetRootTarget(pAppTarget);
			if (!ParsePathRegex(&regexPathElem, pPathRegex))
			{
				delete pPathRegex;
				continue;
			}
			pAppTarget->AddPathRegex(pPathRegex);
		}
		return pAppTarget->PathRegexCount() > 0;
	}

	bool ParsePathRegex( QDomElement* pPathRegexElem, CPathRegex* pPathRegex )
	{
		if (!pPathRegexElem->isElement()
			|| (pPathRegexElem->tagName() != "dir" && pPathRegexElem->tagName() != "file"))
		{
			return false;
		}

		//parse sub regex path
		QString tagName = pPathRegexElem->tagName();
		pPathRegex->SetDir(tagName == "dir");

		std::wstring name = pPathRegexElem->attribute("name").toStdWString();
		pPathRegex->SetName(name);

		//解析嵌套的子目录
		if (pPathRegex->IsDir())
		{
			QDomNodeList subPathList = pPathRegexElem->childNodes();
			for (int i = 0; i < subPathList.size(); ++i)
			{
				QDomElement subPathRegexElem = subPathList.at(i).toElement();
				CPathRegex* pSubPathRegex = new CPathRegex;
				pSubPathRegex->SetParent(pPathRegex);
				pSubPathRegex->SetRootTarget(pPathRegex->GetRootTarget());
				if(!ParsePathRegex(&subPathRegexElem, pSubPathRegex))
				{
					delete pSubPathRegex;
					continue;
				}
				pPathRegex->AddSubPathRegex(pSubPathRegex);
			}
		}
        return true;
	}

    bool CTemplateApp::SearchFiles()
	{
        uint64 approotCount = appRoots_.size();
        for(uint32 i = 0; i < approotCount; ++i) {
            appRoots_.at(i)->SearchFiles();
        }
		return true;
	}

	void CTemplateApp::GetSearchedHandles( std::vector<PSIHANDLE>& handles )
	{
		std::vector<CAppRoot*>::iterator vt = appRoots_.begin();
		for(; vt != appRoots_.end(); ++vt)
		{
			std::vector<PSIHANDLE> subHandles;
			(*vt)->GetSearchedHandles(subHandles);
			handles.insert(handles.end(), subHandles.begin(), subHandles.end());
		}
	}

	bool CTemplateApp::ParseTemplate(const std::wstring& templatePath)
    {
		if (!ParseApp(templatePath)) {
            return false;
        }
        return true;
    }

    std::wstring CTemplateApp::GetApplicationName()
    {
        return applicationName_;
    }
}
