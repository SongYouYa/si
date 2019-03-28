#include "stdafx.h"
#include "siutility/PCodeConverter.h"
#include <iconv.h>
namespace siutil
{

    std::wstring CPCodeConverter::Decode(const char* inBuf, int inLen,CODE_TYPE inDecodeType)
    {
        iconv_t cd = (iconv_t)-1;

        char * pInBuf =  (char*)inBuf;
        wchar_t * woutBuf = 0;
        int woutLen = 0;
        size_t nInBufSize = inLen;
        size_t nOutBufSize = nInBufSize * 4;

        woutBuf = new wchar_t[nOutBufSize / sizeof(wchar_t)];

        memset((woutBuf), '\0', nOutBufSize);

        //char * pOutBuf = new char[nOutBufSize];
        char * pOutBuf = (char *)(woutBuf);

    #if defined(__PPC__) || defined(__powerpc__)
        char szToCodec[] = "UCS-4BE";
    #else
        char szToCodec[] = "UCS-4LE";
    #endif
        char szFromCodec[20];
        bzero(szFromCodec, 20);

        switch(inDecodeType)
        {
        case CPCodeConverter::GBK:
        case CPCodeConverter::MBCS:
            strcpy(szFromCodec, "GBK");
            break;

        case CPCodeConverter::UTF16:
            strcpy(szFromCodec, "UTF-16LE");
            break;

        case CPCodeConverter::BUTF16:
            strcpy(szFromCodec, "UTF-16BE");
            break;

        case CPCodeConverter::UTF8:
            strcpy(szFromCodec, "UTF-8");
            break;

        case CPCodeConverter::UTF7:
            strcpy(szFromCodec, "UTF-7");
            break;

        case CPCodeConverter::BIG5:
            strcpy(szFromCodec, "BIG5");
            break;

        default:
            return L"";
        }

        cd = iconv_open(szToCodec, szFromCodec);

        iconv(cd, &pInBuf, &nInBufSize, &pOutBuf, &nOutBufSize);

        woutLen = (inLen * 4 - nOutBufSize) / 4;

        iconv_close(cd);
        std::wstring strValue = std::wstring(woutBuf,woutLen);
        delete []woutBuf;
        return strValue;
    }

    bool CPCodeConverter::IsCodeUTF8(const char* source_str)
    {
         std::wstring wstrUnicode = CPCodeChanger::UTF8ToUnicode(source_str);
         std::string  strUtf = CPCodeChanger::UnicodeToUTF8(wstrUnicode.c_str());
         if (strcmp(source_str, strUtf.c_str()) == 0)
         {
             return true;
         }
         else
         {
             return false;
         }
    }
}
