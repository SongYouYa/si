#include "stdafx.h"
#include "PSplitDdFile.h"

#include <boost/algorithm/string.hpp>

namespace siio
{
	class CPSplitDdFile::Impl
	{
	public:
		Impl()
		{

		}

		CPNormalFile _file;
	};

	CPSplitDdFile::CPSplitDdFile(uint64 splitSize)
		: _impl(new Impl), _splitSize(splitSize), 
		_offset(0),
		_openMode(0)
	{

	}

	CPSplitDdFile::~CPSplitDdFile()
	{
		if (_impl) {
			delete _impl;
			_impl = nullptr;
		}
	}

	bool CPSplitDdFile::open(const std::wstring& path, int openMode)
	{
		if (_splitSize == 0) {
			return false;
		}

//		std::wstring tempPath = path;
//#ifdef OS_PWINDOWS
//		//转换/，以便处理
//		boost::replace_all(tempPath, L"\\", L"/");
//#endif

		boost::filesystem::path p(path);
		_ext = p.extension().wstring();
		_basePath = path.substr(0, path.size() - _ext.size());
		_openMode = openMode;

		return true;
	}

	bool CPSplitDdFile::openCurSplit(bool bNew)
	{
		uint64 num = _offset / _splitSize;

		std::wstring splitPath;
		if (num == 0) {
			//首个文件为原始文件名
			splitPath = _basePath + _ext;
		}
		else {
			splitPath = _basePath + L"_" + std::to_wstring(num) + _ext;
		}

		_impl->_file.close();

		if (bNew) {
			if (boost::filesystem::exists(splitPath)) {
				::remove(W2LS(splitPath).c_str());
			}
		}

		if (!_impl->_file.open(splitPath, _openMode)) {
			return false;
		}
		//调整当前分片文件的偏移
		uint64 curOffset = _offset % _splitSize;
		if (!_impl->_file.seek(curOffset, PSEEK_BEG)) {
			return false;
		}
		return true;
	}

	CPFile::ErrorCode CPSplitDdFile::read(void* buf, uint length, uint* readed)
	{
		return CPFile::PERROR_UNKOWN;
	}

	CPFile::ErrorCode CPSplitDdFile::write(const void* buf, uint length, uint* writed)
	{
		uint totalWrited = 0;
		while (totalWrited < length) {
			uint64 curSplitLeft = _splitSize - (_offset % _splitSize);
			if (curSplitLeft == _splitSize) {
				if (!openCurSplit(true)) {
					return CPFile::PERROR_UNKOWN;
				}
			}
			uint toWrite = (uint)pmin(curSplitLeft, length - totalWrited);
			CPFile::ErrorCode ret = _impl->_file.write((char*)buf + totalWrited, toWrite);
			if (ret != CPFile::PERROR_SUCCESS) {
				return ret;
			}
			totalWrited += toWrite;
			_offset += toWrite;
		}
		if (writed) {
			*writed = totalWrited;
		}

		return CPFile::PERROR_SUCCESS;
	}

	bool CPSplitDdFile::seek(uint64 offset, PSeekFrom from, uint64* seeked)
	{
		bool ret = true;
		if (PSEEK_BEG == from) {
			_offset = offset;
		}
		else if (PSEEK_CUR == from) {
			_offset += offset;
		}
		else {
			ret = false;
		}

		if (seeked) {
			*seeked = _offset;
		}

		if (!ret) {
			return false;
		}

		if (!openCurSplit(false)) {
			return false;
		}
		return true;
	}

	bool CPSplitDdFile::flush()
	{
		return _impl->_file.flush();
	}

	void CPSplitDdFile::close()
	{
		_impl->_file.close();
	}
}