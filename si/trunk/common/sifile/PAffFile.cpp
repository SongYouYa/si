#include "stdafx.h"
#include "sifile/PAffFile.h"
#include "./afflib/afflib.h"
#include "./afflib/afflib_i.h"

#include "sicommon/PCodeChanger.h"

#include <fcntl.h>

namespace siio
{
	class CPAffFile::Impl
	{
	public:
		Impl(int level);
		~Impl();
		bool open(const std::wstring& path, int openMode);
		CPFile::ErrorCode read(void* buf, uint length, uint* readed);
		CPFile::ErrorCode write(const void* buf, uint length, uint* writed);
		bool seek(uint64 offset, PSeekFrom from, uint64* seeked = NULL);
		void close();

	private:
		int _compressType;
		int _compressLevel;
		AFFILE* _af;
	};

	CPAffFile::Impl::Impl(int level)
		: _af(nullptr)
	{
		//不知为何lzma方式没有zlib方式压缩的好
		//是因为aff库里的lzma的实现方式的问题吗？
		if (level == 0) {
			_compressType = AF_COMPRESSION_ALG_NONE;
		}
		else {
			_compressType = AF_COMPRESSION_ALG_ZLIB;
		}
		_compressLevel = level;
	}

	CPAffFile::Impl::~Impl()
	{
		close();
	}

	bool CPAffFile::Impl::open(const std::wstring& path, int openMode)
	{
		std::string strPath = W2LS(path);
		int flags = O_BINARY;
		if (openMode == MODE_READ) {
			flags |= O_RDONLY;
		}
		else {
			flags |= O_RDWR;
			flags |= O_CREAT;
			flags |= O_TRUNC; //删除之前的内容
		}
		_af = af_open(strPath.c_str(), flags, 0666);
		if (!_af) {
			return false;
		}

		af_enable_compression(_af, _compressType, _compressLevel);
		af_set_pagesize(_af, 1024 * 1024);//不太清楚设置pagesize的作用，但设置了会减小压缩后的大小

		return true;
	}

	CPFile::ErrorCode CPAffFile::Impl::read(void* buf, uint length, uint* readed)
	{
		int64 af_readed = af_read(_af, (uchar*)buf, length);
		if (af_readed < 0) {
			return PERROR_UNKOWN;
		}
		if (readed) {
			*readed = (uint)af_readed;
		}
		return CPFile::PERROR_SUCCESS;
	}

	CPFile::ErrorCode CPAffFile::Impl::write(const void* buf, uint length, uint* writed)
	{
		int af_writed = af_write(_af, (uchar*)buf, length);
		if (af_writed < 0) {
			return PERROR_UNKOWN;
		}
		if (writed) {
			*writed = (uint)af_writed;
		}
		return CPFile::PERROR_SUCCESS;
	}

	bool CPAffFile::Impl::seek(uint64 offset, PSeekFrom from, uint64* seeked)
	{
		uint64 af_seeked = af_seek(_af, offset, from);
		if (seeked) {
			*seeked = af_seeked;
		}
		return true;
	}
	void CPAffFile::Impl::close()
	{
		if (_af) {
			af_close(_af);
			_af = nullptr;
		}
	}


	CPAffFile::CPAffFile(int level)
		: _impl(new Impl(level))
	{

	}
	CPAffFile::~CPAffFile()
	{
		delete _impl;
	}

	bool CPAffFile::open(const std::wstring& path, int openMode)
	{
		return _impl->open(path, openMode);
	}

	CPFile::ErrorCode CPAffFile::read(void* buf, uint length, uint* readed)
	{
		return _impl->read(buf, length, readed);
	}

	CPFile::ErrorCode CPAffFile::write(const void* buf, uint length, uint* writed)
	{
		return _impl->write(buf, length, writed);
	}

	bool CPAffFile::seek(uint64 offset, PSeekFrom from, uint64* seeked)
	{
		return _impl->seek(offset, from);
	}

	void CPAffFile::close()
	{
		_impl->close();
	}
}
