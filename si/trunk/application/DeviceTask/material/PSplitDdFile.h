#pragma once
#include "siio/PFile.h"

namespace siio
{
	//封装分片的DD文件，以便当做一个抽象文件来操作
	class CPSplitDdFile : public CPFile
	{
	public:
		CPSplitDdFile(uint64 splitSize);
		~CPSplitDdFile();

		bool open(const std::wstring& path, int openMode = MODE_WRITE);
		ErrorCode read(void* buf, uint length, uint* readed = nullptr);
		ErrorCode write(const void* buf, uint length, uint* writed = nullptr);
		bool seek(uint64 offset, PSeekFrom from, uint64* seeked = nullptr);
		bool flush();
		void close();

	private:
		bool openCurSplit(bool bNew);

		class Impl;
		Impl* _impl;

		uint64 _splitSize;
		uint64 _offset;

		std::wstring _basePath; //不带后缀名的路径
		std::wstring _ext;
		int _openMode;
	};
}