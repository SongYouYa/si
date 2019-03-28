#pragma once

namespace report
{
	inline std::string I2S(int value)
	{
		return boost::lexical_cast<std::string>(value);
	}

	inline std::wstring I2WS(int value)
	{
		std::wstring wstr = boost::lexical_cast<std::wstring>(value);
		return wstr;
	}

	inline std::string UI2S(uint64 value)
	{
		return boost::lexical_cast<std::string>(value);
	}
}