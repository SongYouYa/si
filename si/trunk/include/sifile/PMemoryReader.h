#pragma once
#include "./globals.h"
#include "siio/PFile.h"

namespace siio
{
    class SIFILE_API CPMemoryReader : public CPFile
    {
        class Impl;
    public:
        CPMemoryReader();
        ~CPMemoryReader();
        bool initPmemDriver();
        bool open(const std::wstring& path = L"", int openMode = MODE_READ);
        uint64 size();
       //0:³É¹¦,-1:»µµÀ
        CPFile::ErrorCode read(void* buf, uint len, uint* readed = nullptr);
        CPFile::ErrorCode write(const void* buf, uint length, uint* writed = nullptr) {
            return CPFile::PERROR_UNKOWN;
        }
        bool seek(uint64 len, PSeekFrom from, uint64* seeked = nullptr);
        bool flush() {
            return false;
        }
        void close();

        static void setDisplayName(const std::wstring& displayName);
        static std::wstring getDisplayName();
#ifdef __APPLE__
        static void setDriverPath(const std::wstring &path);
#endif

    private:
        Impl* _impl;
        static std::wstring s_displayName;
#ifdef __APPLE__
        static std::wstring  s_macDriverPath;
#endif
    };
    typedef std::shared_ptr<CPMemoryReader> CPMemoryReaderPtr;
}
