#pragma once
#include "./globals.h"
#include "siio/PFile.h"

namespace siio
{
	class SIFILE_API CPAffFile : public CPFile
	{
	public:
		//0-9: 0不压缩，9压缩最好
		CPAffFile(int level = 1);
		~CPAffFile();

		bool open(const std::wstring& path, int openMode = MODE_READ | MODE_WRITE);
		//正确返回0， 错误返回-1
		ErrorCode read(void* buf, uint length, uint* readed = nullptr);
		//正确返回0， 错误返回-1
		ErrorCode write(const void* buf, uint length, uint* writed = nullptr);
		bool seek(uint64 offset, PSeekFrom from, uint64* seeked = NULL);
		bool flush(){ return false; } //TODO:
		void close();

	private:
		class Impl;
		Impl* _impl;
	};
}