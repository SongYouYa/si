#ifndef PLISTHPP_PLISTHPP_H
#define PLISTHPP_PLISTHPP_H

#include <boost/any.hpp>
#include <string>
#include <vector>
#include <map>
#include <stdint.h>
#include "PlistDate.hpp"
#include "common.h"

namespace plistcpp {

	typedef std::string                          string_type;
	typedef int64_t                              integer_type;
	typedef double                               real_type;
	typedef std::map<std::string, boost::any>    dictionary_type;
	typedef std::vector<boost::any>              array_type;
	typedef Date                                 date_type;
	typedef std::vector<char>                    data_type;
	typedef bool                                 boolean_type;

	class PLISTCPP_API CPlistCpp
	{
	public:
		CPlistCpp();
		~CPlistCpp();

		static void ReadPlist(const char* filename, dictionary_type& dict);
		static void ReadPlist(const char* filename, array_type& arr);
		static void ReadPlist(const char* filename, string_type& str);
		static void ReadPlist(const char* filename, data_type& data);
		static void ReadPlist(const char* filename, real_type& real);
		static void ReadPlist(const char* filename, integer_type& intg);
		static void ReadPlist(const char* filename, boolean_type& bl);
		static void ReadPlist(const char* filename, date_type& date);

#ifdef _MSC_VER
		static void ReadPlist(const wchar_t* filename, dictionary_type& dict);
		static void ReadPlist(const wchar_t* filename, array_type& arr);
		static void ReadPlist(const wchar_t* filename, string_type& str);
		static void ReadPlist(const wchar_t* filename, data_type& data);
		static void ReadPlist(const wchar_t* filename, real_type& real);
		static void ReadPlist(const wchar_t* filename, integer_type& intg);
		static void ReadPlist(const wchar_t* filename, boolean_type& bl);
		static void ReadPlist(const wchar_t* filename, date_type& date);
#endif

	};

}

#endif