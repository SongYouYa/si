#pragma once


namespace lic
{
	class CPSiLicChecker
	{
	public:
		//��ȡU�̷����Ĺ��ص�
		static bool getUsbMount(const std::wstring& usbModel,
			std::wstring& mount);
		//releaseDate: __DATE__
		//relaLicPath: ��Ŀ¼������ļ�·��, L"/conf/license_unlocker.bin";
		//relaCrtPath: ��Ŀ¼��֤���ļ�·��, L"/conf/safecheck.crt";
		static bool check(const std::string& releaseDate,
			const std::string& productId,
			const std::wstring& usbModel,
			const std::wstring& relaLicPath,
			const std::wstring& relaCrtPath,
			int& errorCode, std::wstring& errorMsg);
	};
}
