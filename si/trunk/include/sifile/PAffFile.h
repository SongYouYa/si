#pragma once
#include "./globals.h"
#include "siio/PFile.h"

namespace siio
{
	class SIFILE_API CPAffFile : public CPFile
	{
	public:
		//0-9: 0��ѹ����9ѹ�����
		CPAffFile(int level = 1);
		~CPAffFile();

		bool open(const std::wstring& path, int openMode = MODE_READ | MODE_WRITE);
		//��ȷ����0�� ���󷵻�-1
		ErrorCode read(void* buf, uint length, uint* readed = nullptr);
		//��ȷ����0�� ���󷵻�-1
		ErrorCode write(const void* buf, uint length, uint* writed = nullptr);
		bool seek(uint64 offset, PSeekFrom from, uint64* seeked = NULL);
		bool flush(){ return false; } //TODO:
		void close();

	private:
		class Impl;
		Impl* _impl;
	};
}