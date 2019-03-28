#pragma once
#include "siregistry/PRegFile.h"

namespace app
{
	//通过filesystemshell来读取注册表文件
	class CPFsRegFile : public reg::CPRegFile
	{
	public:
		CPFsRegFile(PSIHANDLE handle);
		bool read(uint64 offset, void* buf, uint length, uint* readed = nullptr);

	private:
		PSIHANDLE _handle;
	};
	typedef std::shared_ptr<CPFsRegFile> CPFsRegFilePtr;
}