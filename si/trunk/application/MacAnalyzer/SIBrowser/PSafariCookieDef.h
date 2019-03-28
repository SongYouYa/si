#pragma once
namespace  macbasic
{
template<typename T>
T SwapByte(T tBigEndian)
{
    int iLength = sizeof(T);
    byte temp, *pByte = (byte *)&tBigEndian;
    for (int i = 0; i < iLength/2; i++)
    {
        temp = pByte[i];
        pByte[i] = pByte[iLength - i - 1] ;
        pByte[iLength - i - 1] = temp;
    }
    return tBigEndian;
}

#pragma  pack(1)
const char* const SAFARI_COOKIE_MAGIC = "cook";
const uint    SAFARI_COOKIE_PAGE_HEADER_SIGN = 0x00010000;
const uint    SAFARI_COOKIE_PAGE_HEADER_TAIL = 0x00000000;

struct SAFARI_COOKIES_HEADER
{
    char  Magic[4];
    uint  PageCount;
};

struct SAFARI_COOKIE_PAGE_HEADER
{
    uint    HeadSign;       //! 0x00010000
    uint    RecordCount;    //!本页内 cookie记录的个数
};

struct SAFARI_COOKIE_RECORD
{
    uint    RecordSize;
    uint    Unknown1;
    uint    Unknown2;
    uint    Unknown3;
    uint    UrlOffset;
    uint    NameOffset;
    uint    PathOffset;
    uint    ValueOffset;
    uint64  RecordEndTag;
    double  expiry;          //big endian
    double  LastAccess;     // big endian

};
struct SAFARI_COOKIE_PAGE
{
    uint    PageOffset;
    uint    PageSize;
};
#pragma pack()
}

