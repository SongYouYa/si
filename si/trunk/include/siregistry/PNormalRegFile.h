#pragma once
#include "siregistry/globals.h"
#include "siregistry/PRegFile.h"

#include <fstream>

namespace reg
{
	class CPNormalRegFile : public CPRegFile
	{
	public:
		CPNormalRegFile();
		~CPNormalRegFile();

		bool open(const std::wstring& filePath);

		bool read(uint64 offset, void* buf, uint length, uint* readed = nullptr) override;

		void close();

	private:
		std::ifstream _f;
	};
}