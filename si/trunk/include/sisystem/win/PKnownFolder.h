#pragma once

namespace psystem
{
	class SYSTEM_API CPKnownFolder
	{
		class Impl;
	public:
		CPKnownFolder();
		~CPKnownFolder();

		bool init();
		//ͨ��GUID�ַ�����ȡ�����windowsĿ¼��ʵ��·��
		std::wstring getRealPath(const std::wstring& strGuid);

	private:
		Impl* _impl;
	};
}
