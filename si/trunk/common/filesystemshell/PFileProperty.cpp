#include "stdafx.h"
#include "filesystemshell/PFileProperty.h"
#include "filesystemshell/FileSystemShell.h"

namespace filesystem
{
	static std::mutex g_appMuext;

	CPFileProperty* CPFileProperty::_instance = nullptr;

	CPFileProperty* CPFileProperty::get()
	{
		if (!_instance) {
			_instance = new CPFileProperty();
		}
		return _instance;
	}

	CPFileProperty::CPFileProperty()
		: _isAppParsed(false),
		_isEncParsed(false)
	{

	}

	void CPFileProperty::parseAppFile()
	{
		std::lock_guard<std::mutex> lock(g_appMuext);
		if (!_isAppParsed) {
			_appCallback(_appHandles);
			_isAppParsed = true;
		}
	}

	int CPFileProperty::isAppFile(PSIHANDLE handle)
	{
		if (!_isAppParsed) {
			parseAppFile();
		}
		auto it = _appHandles.find(handle);
		if (it == _appHandles.end()) {
			return 0;
		}
		return 1;
	}

	int CPFileProperty::isEncrypt(PSIHANDLE handle)
	{
		if (!_isEncParsed) {
			_encCallback(_encHandles);
			_isEncParsed = true;
		}
		auto it = _encHandles.find(handle);
		if (it == _encHandles.end()) {
			return 0;
		}
		return 1;
	}

	void CPFileProperty::setParseAppCallback(const ParseCallback& callback)
	{
		_appCallback = callback;
	}

	void CPFileProperty::setParseEncryptCallback(const ParseCallback& callback)
	{
		_encCallback = callback;
	}
}