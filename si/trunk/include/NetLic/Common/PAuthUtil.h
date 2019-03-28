#pragma once

namespace auth
{

	std::string		GetRandomString( int klen );
	unsigned char	GetValidateCode(unsigned char* data,int length);
	bool			IsValidProduct(unsigned short productID);
	std::string     GetLocalIPV4Address();
	int				GetRandomPort();

    //path
#ifdef __linux__
    FILETIME        Timet2Filetime(time64_t tt);
    time64_t          Filetime2Timet(FILETIME ft);
#endif
	FILETIME		GetExeCreateTime();
    FILETIME        PGetLocalTime();
    int             PCompareFileTime(FILETIME* t1, FILETIME* t2);

    std::string     GetWorkDir();

    //i convert
	int				toi(char* str);

	struct CPScopedLock
	{
        static void InitMutex(CRITICAL_SECTION& cs);
        static void DeleteMutex(CRITICAL_SECTION& cs);

        CPScopedLock(CRITICAL_SECTION& cs);
        ~CPScopedLock();

		CRITICAL_SECTION& m_csRef;
	};
}
