#pragma once
#include "siio/PFile.h"

namespace siio
{
	//��װ��Ƭ��DD�ļ����Ա㵱��һ�������ļ�������
	class CPSplitDdFile : public CPFile
	{
	public:
		CPSplitDdFile(uint64 splitSize);
		~CPSplitDdFile();

		bool open(const std::wstring& path, int openMode = MODE_WRITE);
		ErrorCode read(void* buf, uint length, uint* readed = nullptr);
		ErrorCode write(const void* buf, uint length, uint* writed = nullptr);
		bool seek(uint64 offset, PSeekFrom from, uint64* seeked = nullptr);
		bool flush();
		void close();

	private:
		bool openCurSplit(bool bNew);

		class Impl;
		Impl* _impl;

		uint64 _splitSize;
		uint64 _offset;

		std::wstring _basePath; //������׺����·��
		std::wstring _ext;
		int _openMode;
	};
}