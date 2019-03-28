#pragma once
#include "./globals.h"

namespace pfile
{
	class FILE_ANA_API CPEncryptFile
	{
	public:
		CPEncryptFile();
		static void getAllEncryptHandles(std::set<PSIHANDLE>& handles);
	};
}
