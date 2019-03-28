#include "stdafx.h"
#include "./fileoper/PLogger.h"
#include "sicommon/PCodeChanger.h"

namespace devtask
{
	CPLogger::CPLogger(const std::wstring& path)
		: _path(path)
	{

	}

	CPLogger::~CPLogger()
	{

	}

	bool CPLogger::init()//2
	{
		try
		{
			boost::filesystem::path destination_path(_path);
			if (!boost::filesystem::exists(destination_path.parent_path()))
			{
				boost::filesystem::create_directories(destination_path.parent_path()); //exporFiles文件生成时刻
			}

			if (destination_path.filename().wstring() == L".") //fat32或ntfs文件系统下，文件名不能是单个“.”
			{
				return false;
			}
		}
		catch (...)
		{
			return false;
		}

        _out.open(W2LS(_path), std::ios::trunc);//create a log.tex
		if (!_out.is_open()) {
			return false;
		}

		std::wstring header = L"文件名,\tMD5,\t创建时间,\t修改时间,\t访问时间,\t源路径,";
		_out << CPCodeChanger::UnicodeToUTF8(header) << std::endl;
		return true;
	}
	void CPLogger::close()
	{
		_out.close();
	}

	void CPLogger::add(std::wstring& itemName, const std::wstring& itemPath,
		const std::string& md5,
		const std::wstring& createTime,
		const std::wstring& modifyTime,
		const std::wstring& accessTime)
	{
		_out << W2U8(itemName) << ",\t"
			<< md5 << ",\t"
			<< W2U8(createTime) << ", " << W2U8(modifyTime) << ", " << W2U8(accessTime) << ",\t"
			<< W2U8(itemPath)
			<< std::endl;
	}
}
