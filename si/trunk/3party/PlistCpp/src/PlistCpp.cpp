#include "PlistCpp.h"
#include "Plist.hpp"


namespace plistcpp
{
	CPlistCpp::CPlistCpp()
	{

	}

	CPlistCpp::~CPlistCpp()
	{

	}

	void CPlistCpp::ReadPlist( const char* filename, dictionary_type& dict )
	{
		readPlist(filename, dict);
	}

	void CPlistCpp::ReadPlist( const char* filename, array_type& arr )
	{
		readPlist(filename, arr);
	}

	void CPlistCpp::ReadPlist( const char* filename, string_type& str )
	{
		readPlist(filename, str);
	}

	void CPlistCpp::ReadPlist( const char* filename, data_type& data )
	{
		readPlist(filename, data);
	}

	void CPlistCpp::ReadPlist( const char* filename, real_type& real )
	{
		readPlist(filename, real);
	}

	void CPlistCpp::ReadPlist( const char* filename, integer_type& intg )
	{
		readPlist(filename, intg);
	}

	void CPlistCpp::ReadPlist( const char* filename, boolean_type& bl )
	{
		readPlist(filename, bl);
	}

#ifdef _MSC_VER
	void CPlistCpp::ReadPlist( const wchar_t* filename, dictionary_type& dict )
	{
		readPlist(filename, dict);
	}

	void CPlistCpp::ReadPlist( const wchar_t* filename, array_type& arr )
	{
		readPlist(filename, arr);
	}

	void CPlistCpp::ReadPlist( const wchar_t* filename, string_type& str )
	{
		readPlist(filename, str);
	}

	void CPlistCpp::ReadPlist( const wchar_t* filename, data_type& data )
	{
		readPlist(filename, data);
	}

	void CPlistCpp::ReadPlist( const wchar_t* filename, real_type& real )
	{
		readPlist(filename, real);
	}

	void CPlistCpp::ReadPlist( const wchar_t* filename, integer_type& intg )
	{
		readPlist(filename, intg);
	}

	void CPlistCpp::ReadPlist( const wchar_t* filename, boolean_type& bl )
	{
		readPlist(filename, bl);
	}

#endif

}

//}