#include "stdafx.h"
#include "sifile/PMemoryReader.h"

namespace siio
{
	std::wstring CPMemoryReader::s_displayName;

	typedef int(*OpenFunc)();
	typedef unsigned __int64(*SizeFunc)();
	typedef int(*ReadFunc)(unsigned __int64, unsigned int, void*, unsigned int*);
	typedef void(*CloseFunc)();

	class CPMemoryReader::Impl
	{
	public:
		Impl()
			: _module(NULL),
			_openFunc(NULL),
			_sizeFunc(NULL),
			_readFunc(NULL),
			_closeFunc(NULL)
		{}

		bool init();
		void uninit();

		HMODULE _module;
		OpenFunc _openFunc;
		SizeFunc _sizeFunc;
		ReadFunc _readFunc;
		CloseFunc _closeFunc;

		uint64 _offset;
	};

	bool CPMemoryReader::Impl::init()
	{
		if (_closeFunc) {
			return true;
		}
		WCHAR exePath[MAX_PATH + 1] = { 0 };
		::GetModuleFileName(NULL, (LPWSTR)exePath, MAX_PATH);
		std::wstring strExePath(exePath);
		size_t pos = strExePath.rfind(L"\\");
		std::wstring strPmemPath = strExePath.substr(0, pos) + L"\\MemoryReader.dll";
		_module = ::LoadLibrary(strPmemPath.c_str());
		if (!_module) {
			DWORD err = GetLastError();
			return false;
		}

		_openFunc = (OpenFunc)::GetProcAddress(_module, "open_memory");
		if (!_openFunc) {
			return false;
		}
		_sizeFunc = (SizeFunc)::GetProcAddress(_module, "memory_size");
		if (!_sizeFunc) {
			return false;
		}
		_readFunc = (ReadFunc)::GetProcAddress(_module, "read_memory");
		if (!_readFunc) {
			return false;
		}
		_closeFunc = (CloseFunc)::GetProcAddress(_module, "close_memory");
		if (!_closeFunc) {
			return false;
		}

		return true;
	}

	void CPMemoryReader::Impl::uninit()
	{
		if (_module) {
			FreeLibrary(_module);
			_module = NULL;
			_openFunc = NULL;
			_sizeFunc = NULL;
			_readFunc = NULL;
			_closeFunc = NULL;
		}
	}

	CPMemoryReader::CPMemoryReader()
		: _impl(new Impl)
	{

	}

	CPMemoryReader::~CPMemoryReader()
	{
		close();

		if (_impl) {
			delete _impl;
			_impl = NULL;
		}
	}

	bool CPMemoryReader::open(const std::wstring& path, int openMode)
	{
		if (!_impl->init()) {
			return false;
		}
		if (_impl->_openFunc) {
			return _impl->_openFunc() == 0;
		}
		return false;
	}

	uint64 CPMemoryReader::size()
	{
		if (_impl->_openFunc) {
			return _impl->_sizeFunc();
		}
		return 0;
	}

	CPFile::ErrorCode CPMemoryReader::read(void* buf, uint len, uint* readed)
	{
		if (_impl->_readFunc) {
			uint tempReaded = 0;
			int ret = _impl->_readFunc(_impl->_offset, len, buf, &tempReaded);
			if (ret == 0) {
				_impl->_offset += tempReaded;
				if (readed) {
					*readed = tempReaded;
				}
				return CPFile::PERROR_SUCCESS;
			}
		}
		return CPFile::PERROR_UNKOWN;
	}

	bool CPMemoryReader::seek(uint64 len, PSeekFrom from, uint64* seeked)
	{
		switch (from)
		{
		case PSEEK_BEG:
			_impl->_offset = len;
			break;
		case PSEEK_CUR:
			_impl->_offset += len;
			break;
		case PSEEK_END:
			_impl->_offset = size() - len;
			break;
		default:
			break;
		}
		return _impl->_offset <= size();
	}

	void CPMemoryReader::close()
	{
		if (_impl->_closeFunc) {
			_impl->_closeFunc();
		}
		_impl->uninit();
	}

	void CPMemoryReader::setDisplayName(const std::wstring& displayName)
	{
		s_displayName = displayName;
	}

	std::wstring CPMemoryReader::getDisplayName()
	{
		return s_displayName;
	}
}