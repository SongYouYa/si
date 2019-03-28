#pragma once


class CPTimeParser
{
public:
	//两种方法，获取发布时间
	//__DATE__:编译时间(Jun  7 2018)转为如20180607
	static std::string getReleaseDate(const std::string& releaseDate);
	static std::string getCurrentTime();
};
