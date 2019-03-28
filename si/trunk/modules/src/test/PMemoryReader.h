#pragma once
#include "siio/PFile.h"

namespace siio
{
	class CPMemoryReader
	{
		class Impl;
	public:
		CPMemoryReader();
		~CPMemoryReader();

		bool open(const std::wstring& path);
		uint64 size();
		//0:³É¹¦,-1:»µµÀ
		CPFile::ErrorCode read(void* buf, uint len, uint* readed = NULL);
		bool seek(uint64 len, PSeekFrom from, uint64* seeked = NULL);
		void close();

	private:
		Impl* _impl;
	};
	typedef std::shared_ptr<CPMemoryReader> CPMemoryReaderPtr;
}
