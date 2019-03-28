#pragma once


namespace lic
{
	class CPSiLicChecker
	{
	public:
		//获取U盘分区的挂载点
		static bool getUsbMount(const std::wstring& usbModel,
			std::wstring& mount);
		//releaseDate: __DATE__
		//relaLicPath: 卷目录下许可文件路径, L"/conf/license_unlocker.bin";
		//relaCrtPath: 卷目录下证书文件路径, L"/conf/safecheck.crt";
		static bool check(const std::string& releaseDate,
			const std::string& productId,
			const std::wstring& usbModel,
			const std::wstring& relaLicPath,
			const std::wstring& relaCrtPath,
			int& errorCode, std::wstring& errorMsg);
	};
}
