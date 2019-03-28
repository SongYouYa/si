#ifndef SIENTERPRISE_UTILITY_CODECONVERTER_H_
#define SIENTERPRISE_UTILITY_CODECONVERTER_H_

namespace siutil
{
    class  CPCodeConverter
    {
    public:

        enum CODE_TYPE
        {
            MBCS	= 0x00,		//MBCS
            UTF16	= 0x01,
            BUTF16	= 0x02,		//big endian unicode
            UTF8	= 0x03,
            UTF7	= 0x04,
            BASE64	= 0x05,
            GBK		= 0x06,
            BIG5	= 0x07,
        };

        static bool IsCodeUTF8(const char* source_str);
        static std::wstring Decode(const char* inBuf, int inLen,CODE_TYPE inDecodeType);
    };
}
#endif
