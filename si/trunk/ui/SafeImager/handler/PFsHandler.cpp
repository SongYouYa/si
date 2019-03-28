#include "stdafx.h"
#include "PFsHandler.h"

#include "logic/PDeviceLoader.h"

#include "filesystemshell/FileSystemShell.h"

using namespace filesystem;

CPFsHandler* CPFsHandler::_instance = nullptr;

CPFsHandler* CPFsHandler::get()
{
	if (!_instance) {
		_instance = new CPFsHandler();
	}
	return _instance;
}

CPFsHandler::CPFsHandler()
	:_fsLoaded(false)
{

}

void CPFsHandler::loadFs()
{
	if (_fsLoaded) {
		return;
	}
	_fsLoaded = true;
    std::wstring exeDir;
#ifdef OS_PAPPLE
    exeDir = LS2W(CPUtils::getMacAppDir()) + L"/Contents/dylibs";;
#else
    exeDir = CPUtils::getAppDir();
#endif
    bool ret = CFileSystem::Initialize(exeDir);
	if (!ret) {
		CPMessageBox::warning(tr("Init filesystem failed"));
		return;
	}
	CPDeviceLoader::get()->loadAllDevices();
}
