#pragma once
#include "siregistry/PRegFile.h"

namespace app
{
	//ͨ��filesystemshell����ȡע����ļ�
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