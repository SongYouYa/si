#include "stdafx.h"

#include "PMemoryReader.h"
#include "siio/PNormalFile.h"

using namespace siio;

int main()
{
	CPMemoryReader mreader;
	if (!mreader.open(L"")) {
		return 1;
	}
	CPNormalFile output;
	if (!output.open(L"memory.dd")) {
		return 1;
	}

	uint64 msize = mreader.size();
	const uint BUF_SIZE = 4 * 1024 * 1024;
	std::vector<char> vbuf(BUF_SIZE, 0);
	char* buf = vbuf.data();
	uint64 totalReaded = 0;
	uint readed = 0;
	while (totalReaded < msize) {
		uint toRead = (uint)min(BUF_SIZE, msize - totalReaded);
		int ret = mreader.read(buf, toRead, &readed);
		if (CPFile::PERROR_SUCCESS != ret) {
			break;
		}
		ret = output.write(buf, readed);
		if (CPFile::PERROR_SUCCESS != ret) {
			break;
		}
		totalReaded += readed;
	}
	output.close();
	mreader.close();
}