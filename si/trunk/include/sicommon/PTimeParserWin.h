#pragma once

class CPTimeParserWin
{
public:
	//获取程序修改日期
	static std::string getReleaseDate();
	//获取当前日期
	static std::string getCurrentDate();
	//获取EXE路径
	static std::string getExePath();
};
