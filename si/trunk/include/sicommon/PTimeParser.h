#pragma once


class CPTimeParser
{
public:
	//���ַ�������ȡ����ʱ��
	//__DATE__:����ʱ��(Jun  7 2018)תΪ��20180607
	static std::string getReleaseDate(const std::string& releaseDate);
	static std::string getCurrentTime();
};
