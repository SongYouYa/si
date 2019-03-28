#include "stdafx.h"
#include "sifile/PMemoryReader.h"

namespace siio
{
	std::wstring CPMemoryReader::s_displayName;

    class CPMemoryReader::Impl
    {
    public:
        Impl()
        {}
    };

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
		return false;
	}

	uint64 CPMemoryReader::size()
	{
		return 0;
	}

	CPFile::ErrorCode CPMemoryReader::read(void* buf, uint len, uint* readed)
	{
		return CPFile::PERROR_UNKOWN;
	}

	bool CPMemoryReader::seek(uint64 len, PSeekFrom from, uint64* seeked)
	{
            return false;
	}

	void CPMemoryReader::close()
	{
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
