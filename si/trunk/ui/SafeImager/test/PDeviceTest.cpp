#include "stdafx.h"

#include "siio/PDeviceReader.h"
#include "sifile/PAffFile.h"

using namespace siio;

void test_aff_image()
{
	std::clock_t st = std::clock();

	CPDeviceReader reader;
	if (!reader.open(L"\\\\.\\PhysicalDrive6")) {
		return;
	}
	CPAffFile affImage(1);
	if (!affImage.open(L"F:\\disk6.aff")) {
		return;
	}

	uint64 sourceSize = reader.size();
	const uint BUF_SIZE = 1 * 1024 * 1024;
	std::vector<char> vbuf(BUF_SIZE, 0);
	char* buf = vbuf.data();

	uint64 totalReaded = 0;
	uint readed = 0;
	while (totalReaded < sourceSize) {
		uint toRead = pmin(sourceSize - totalReaded, BUF_SIZE);
		if (reader.read(buf, BUF_SIZE, &readed) != CPFile::PERROR_SUCCESS) {
			break;
		}
		uint writed = 0;
		if (affImage.write(buf, readed, &writed) != CPFile::PERROR_SUCCESS) {
			break;
		}

		totalReaded += readed;
	}

	affImage.close();
	reader.close();

	std::clock_t et = std::clock();
	std::clock_t p = et - st;
}

void test_device()
{
	test_aff_image();
}