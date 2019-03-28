#pragma once


namespace devtask
{
	class CPLogger
	{
	public:
		CPLogger(const std::wstring& path);
		~CPLogger();

		bool init();
		void close();

		void add(std::wstring& itemName, const std::wstring& itemPath,
			const std::string& md5,
			const std::wstring& createTime,
			const std::wstring& modifyTime,
			const std::wstring& accessTime);

	private:
		std::wstring _path;
		std::ofstream _out;
	};
}
