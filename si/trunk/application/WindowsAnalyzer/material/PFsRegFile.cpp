#include "stdafx.h"
#include "PFsRegFile.h"


namespace app
{
	CPFsRegFile::CPFsRegFile(PSIHANDLE handle)
		: _handle(handle)
	{

	}

	bool CPFsRegFile::read(uint64 offset, void* buf, uint length, uint* readed)
	{
		uint64 fsReaded = CFileSystem::ReadFile(_handle, offset, length, (uchar*)buf);
		if (fsReaded == length) {
			if (readed) {
				*readed = (uint)fsReaded;
			}
			return true;
		}
		return false;
	}
}