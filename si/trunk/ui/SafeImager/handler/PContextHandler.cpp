#include "stdafx.h"
#include "PContextHandler.h"

#ifdef SI_VER2_CD
#include "ui/net/PNetShareDialog.h"
#endif
#include "silicchecker/PSiLicChecker.h"
#include "PLicDefs.h"

CPContextHandler::CPContextHandler(QObject* parent)
    : QObject(parent)
{

}

bool CPContextHandler::initContext()
{
    CPAppContext* ctx = CPAppContext::get();
	if (!ctx->opath(L"").empty()) {
		return true;
	}
#ifdef SI_VER2_CD
    //ctx->setOutDir(L"");
    CPNetShareDialog netDialog;
    int ret = netDialog.exec();
    std::wstring mountDir = netDialog.getMountDir();
    if (ret == CPDialog::Accepted && !mountDir.empty()) {
        ctx->setOutDir(mountDir);
		return true;
    }
#else
    std::wstring outRoot;
	//找SIU盘，作为工作输出目录
    if (lic::CPSiLicChecker::getUsbMount(PMODEL, outRoot)) {
        ctx->setOutDir(outRoot);
		return true;
    }
#if !defined(_DEBUG) && 0
	//没有找到SIU盘
	return false;
#endif
	
#ifdef OS_PLINUX
	//从USB设备，离线linux启动目标电脑后，程序分区挂载在/cdrom下
	const QString confIni = "/conf/build.ini";
	QString rootDir1 = "/isodevice"; //从iso文件启动时，iso文件所在的分区
	QString rootDir2 = "/cdrom"; //U盘有时会挂载到cdrom下
	if (QFile(rootDir1 + confIni).exists()) {
		outRoot = rootDir1.toStdWString();
	}
	else if (QFile(rootDir2 + confIni).exists()) {
		outRoot = rootDir2.toStdWString();
	}
#if defined(_DEBUG) || 1
    else {
        outRoot = CPUtils::getAppRootDir();
    }
#endif
#else
	outRoot = CPUtils::getAppRootDir();
#endif
    if (!outRoot.empty()) {
        CPAppContext::get()->setOutDir(outRoot);
		return true;
    }
#endif
	return false;
}
