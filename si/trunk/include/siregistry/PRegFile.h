#pragma once


namespace reg
{
	class CPRegFile
	{
	public:
		CPRegFile() {};
		virtual ~CPRegFile() {};
		virtual bool read(uint64 offset, void* buf, uint length, uint* readed = nullptr) = 0;
	};

	typedef std::shared_ptr<CPRegFile> CPRegFilePtr;
}