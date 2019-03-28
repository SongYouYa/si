#ifndef SI_FS_FILESYSTEM_HELPER_H
#define SI_FS_FILESYSTEM_HELPER_H
#include "./globals.h"
#include "./PInterfaceStruct.h"

namespace filesystem
{
	//SI�ļ�ϵͳ����Ľṹ������
	class  FSS_API CFileSystem
	{
	public:
		/*{=====================================================*//**
		@brief		�ļ�ϵͳ��ʼ��
		@return		bool��		�ɹ�����true
		@author		������
		@date		2014.10.15
		@remark		��Ҫ����DLL,��������غ���
		*///}========================================================
        static bool Initialize(const std::wstring& dllDir);
		/*{=====================================================*//**
		@brief		�õ������豸�����̣�������
		@param		��
		@return		int��					�����豸����Ŀ
		@author		������
		@date		2014.10.15
		@remark		����д����򷵻� -1
		*///}========================================================
		static int LocalDeviceCount();
		/*{=====================================================*//**
		@brief		�õ��豸����Ϣ
		@param		[����]��	index��	�豸������
		@param		[���]��	pItem��	�豸��Ϣ
		@return		PSIHANDLE��       �豸�ľ��
		@author		������
		@date		2008.10.15
		@remark
		*///}========================================================
		static PSIHANDLE GetDeviceInfo(unsigned int index, SPDevice* pDevice);
		/*{=====================================================*//**
		@brief		 ͨ������õ��豸����Ϣ
		@param		[����]��	hDevice���豸�ľ��
		@param		[���]��	pItem��	�豸��Ϣ
		@return		bool : �ɹ�����ture
		@author		������
		@date		2008.10.27
		@remark
		*///}========================================================
		static bool GetDeviceInfo(const PSIHANDLE hDevice, SPDevice* pItem);
		/*{=====================================================*//**
		@brief		�����豸�������ļ�ϵͳ
		@param		[����]��	hDevice��		�����豸�ľ��
		@return		bool��		��������true�����򷵻�false
		@author		������
		@date		2014.10.15
		@remark		��ע�⣺�������������ļ��Ķ��٣�Ҫ���ѱȽϳ���ʱ��
		*///}========================================================
		static bool LoadDevice(PSIHANDLE hDevice);
		/*{=====================================================*//**
		@brief		�ͷ�ָ���豸
		@param		[����]��    hItem��     �豸�ľ��
		@return		void
		@author		������
		@date		2014.10.15
		@remark		�ͷ��豸��ɾ���豸�µľ���ļ�
		*///}========================================================
		static bool ReleaseDevice(PSIHANDLE hDevice);
		/*{=====================================================*//**
		@brief		�ͷ������豸
		@param		[����]��    hItem��     �豸�ľ��
		@return		void
		@author		������
		@date		2014.10.15
		@remark		�ͷ��豸��ɾ���豸�µľ���ļ�
		*///}========================================================
		static void ReleaseAllDevice();

		/*{=====================================================*//**
		@brief		�����ļ�ϵͳ��ʱ��
		@param		[����]��    hDevice|hVolume��     �豸|��ľ��
		@param		[����]��    bias��ʱ��ƫ������������綫������bias=-480(-8*60);
		@                            ������ ��bias = 480��8*60��
		@return		bool
		@author		������
		@date		2015.10.22
		@remark		�����ļ�ϵͳ��ʱ��,windowsϵͳ���Ѿ�����������ʱ��
		@           linuxϵͳ���Ѿ�������Ĭ�ϵĶ�������bias = -480��
		*///}========================================================
		static bool UpdateDeviceTimeZone(PSIHANDLE hDevice, long bias);
		static bool UpdateVolumeTimeZone(PSIHANDLE hVolume, long bias);

		/*{=====================================================*//**
		@brief		����ĳ��item������
		@param		[����]��	hItem��      ��ǰ���
		@return     ITEM_TYPE��          ����
		@author		������
		@date		2014.10.23
		@remark		����hItem�����ͣ���Ч��hitem����ITEM_TYPE_NONE
		*///}========================================================
		static ITEM_TYPE GetItemType(const PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief      �ж��豸�Ƿ�ΪUSB�豸
		@param		[����]��    hItem��     �豸�ľ��
		@return		bool
		@author		������
		@date		2015.10.14
		@remark		GetLocalDevice���ص��豸�а���USB�豸
		*///}========================================================
		static bool IsUsbDevice(PSIHANDLE hDevice);

		/*{=====================================================*//**
		@brief		�õ������豸�¾������
		@param		[����]��	hDevice��   ��ǰ�豸�ľ��
		@return		int��			    ��ǰ�豸�о����Ŀ
		@author		������
		@date		2014.10.15
		@remark		����д����򷵻� -1
		*///}========================================================
		static int VolumeCount(const PSIHANDLE hDevice);


		/*{=====================================================*//**
		@brief		�õ��߼��������
		@return		int��			    ��ǰ�豸�о����Ŀ
		@author		������
		@date		2014.10.15
		@remark		����д����򷵻� -1
		*///}========================================================
		static int LogicalVolumeCount();

		/*{=====================================================*//**
		@brief		�õ������Ϣ
		@param		[����]��	hDevice��		��ǰ�豸�ľ��
		@param		[����]��	index��		    �������
		@param		[���]��	pItem��		    �����Ϣ
		@return		PSIHANDLE��				��ľ��
		@author		������
		@date		2014.10.15
		@remark
		*///}========================================================

		static PSIHANDLE GetVolumeInfo(const PSIHANDLE hDevice, int index, SPVolume* pVolume);

		/*{=====================================================*//**
		@brief		�õ������Ϣ
		@param		[����]��	hVolume��		��ǰ��ľ��
		@param		[���]��	pItem��		    �����Ϣ
		@return		bool��				    �ɹ�����true
		@author		������
		@date		2014.10.27
		@remark
		*///}========================================================
		static bool GetVolumeInfo(const PSIHANDLE hVolume, SPVolume* pItem);


		/*{=====================================================*//**
		@brief		�õ��߼������Ϣ
		@param		[����]��	index��		�������
		@param		[���]��	pItem��		��Ľṹ
		@return		PHANDLE��		    ��ľ��
		@author		������
		@date		2015.4.3
		@remark     ĳЩͨ��mount���߹��ص����صľ����������������豸����truecrypt��
		@           FTK Imager�ȹ��صľ�����Щ��û�����ƴ��̵ĸ��豸�����ܹ��ص�Դ��
		@           һ�������ļ�����ܵķ����������ﲻ��Ҫͨ�����豸��handle����ȡ
		*///}========================================================
		static PSIHANDLE GetLogicalVolumeInfo(unsigned int index, SPVolume* pItem);

		/*{=====================================================*//**
		@brief		�õ��ļ��������
		@param		[����]��	hParent��		��ǰ��/Ŀ¼�ľ��
		@return		int��					��ǰ��/Ŀ¼�е����ļ������Ŀ
		@author		������
		@date		2014.10.18
		@remark		1����� hParent ��ʾ���򷵻����Ŀ¼�µ��ļ�����Ŀ
		@n			2������ֱ�����ļ�����ݹ������Ŀ¼�е��ļ���
		@n			2������д����򷵻� -1
		*///}========================================================
		static int SubItemCount(const PSIHANDLE hParent);

		/*{=====================================================*//**
		@brief		�õ���/Ŀ¼�µ��ļ���Ϣ
		@param		[����]��	hParent��	��ǰ��/Ŀ¼�ľ��
		@param		[����]��	index��		�ļ��������
		@param		[���]��	pFileItem�� �ļ���Ľṹ
		@return		PSIHANDLE��		    �ļ���ľ��
		@author		������
		@date		2014.10.15
		@remark
		*///}========================================================
		static PSIHANDLE GetFileItem(const PSIHANDLE hParent, unsigned int index, SPFileItem* pFileItem);

		//Ŀǰ������ͨ��ʵ��·���Ҳ���handle
#ifdef OS_PWINDOWS
		/*{=====================================================*//**
		@brief		ͨ��·���õ��ļ���ľ��
		@param		[����]��path��		�ļ���·��
		@return		PSIHANDLE��		�ļ���ľ��
		@author		������
		@date		2015.4.1
		@remark
		*///}========================================================
		static PSIHANDLE GetItemHandleByPath(const std::wstring& path);
#endif

		/*{=====================================================*//*
		@brief		ͨ�����·���õ����о��µ��ļ���ľ��, �����ļ���ͨ����� * ������ʾ·����ĳЩ�ַ���
		@param		[����]��path��		���µ��ļ�������·��
		@param		[����]: CaseSensitive         �Ƿ����ִ�Сд
		@param		[���]��handles��		��ȡ��ƥ����ļ���(ֻ��handles������Ӳ�������������յȲ���)
		@return		bool:                �Ƿ����ҵ��ļ�
		@author		������
		@date		2015.7.31
		@remark
		*///}========================================================
		static bool GetItemHandlesByRawPath(const std::wstring& path, std::vector<PSIHANDLE>& handles, bool CaseSensitive = false);

		/*{=====================================================*//**
		@brief		ͨ�����·���õ�ָ����Ŀ¼�µ��ļ���ľ��
		@param		[����]: pRootHandle      ��Ŀ¼���
		@param		[����]��path��		���µ��ļ�������·��
		@param		[����]: CaseSensitive �Ƿ����ִ�Сд
		@return		PSIHANDLE:         ƥ����ļ���
		@author		������
		@date		2015.8.3
		@remark
		*///}========================================================
		static PSIHANDLE GetItemHandleByRawPath(PSIHANDLE pRootHandle, const std::wstring& path, bool CaseSensitive = false);

		/*{=====================================================*//*
		@brief		ͨ�����·���õ�ָ����Ŀ¼�µ��ļ���ľ��, �����ļ���ͨ����� * ������ʾ·����ĳЩ�ַ���
		@param		[����]: pRootHandle      ��Ŀ¼���
		@param		[����]��path��		��Ŀ¼�µ��ļ�������·��
		@param		[���]��handles��		��ȡ��ƥ����ļ���(ֻ��handles������Ӳ�������������յȲ���)
		@param		[����]: CaseSensitive �Ƿ����ִ�Сд
		@return		PSIHANDLE:         ƥ����ļ���
		@author		������
		@date		2015.8.3
		@remark
		*///}========================================================
		static bool GetItemHandlesByRawPath(PSIHANDLE pRootHandle, const std::wstring& path,
			std::vector<PSIHANDLE>& handles, bool CaseSensitive = false);

		/*{=====================================================*//**
		@brief		��ȡ���о�ľ��, �ѹ���safeimager��
		@param		[���] pVolumes:		��ȡ�ľ���
		@author		������
		@date		2015.8.3
		@remark
		*///}========================================================
		static void GetAllVolumes(std::vector<PSIHANDLE>& pVolumes);

		/*{=====================================================*//**
		@brief		�ж�����handle�Ƿ���ͬһ������
		@param		[����] pItem1:		����
		@param		[����] pItem2:		����
		@author		������
		@date		2015.8.25
		@remark
		*///}========================================================
		static bool InSameVolume(PSIHANDLE pItem1, PSIHANDLE pItem2);

		/*{=====================================================*//**
		@brief		�ж�pItem�Ƿ�pRootItem������(�ݹ�)
		@param		[����]��	pRootItem   �����
		@param		[����]��	pItem   �Ӿ��
		*///}========================================================
		static bool IsSubHandle(const PSIHANDLE pRootItem, PSIHANDLE pItem);

		/*{=====================================================*//**
		@brief		ͨ���ļ��ľ���õ��ļ������Ϣ
		@param		[����]��	hItem��		���ļ�/Ŀ¼�ľ��
		@param		[���]��	pItem��		�ļ���Ľṹ
		@return		bool:               ��������true
		@author		������
		@date		2014.10.20
		@remark		��ȡ�ļ���Ŀ¼����Ϣ����������ļ���Ŀ¼�ľ��������false
		*///}========================================================
		static bool GetFileItemInfo(const PSIHANDLE hItem, SPFileItem* pItem);


		/*{=====================================================*//**
		@brief		��ȡ���ļ�����
		@param		[����]��	hFile��		��ǰ�ļ��ľ��
		@param		[����]��	begin��		�ļ��ڵ�ƫ��
		@param		[����]��	offset�� 	Ҫ��ȡ�ĳ���
		@param		[���]��	buf��		����
		@return		int��				ʵ�ʶ�ȡ�ĳ���
		@author		������
		@date		2014.10.15
		@remark
		*///}========================================================
		static unsigned __int64 ReadFile(
			const PSIHANDLE hFile,
			unsigned __int64 offset,
			unsigned __int64 length,
			unsigned char* buf);

		/*{=====================================================*//**
		@brief		����ĳ��item�Ĵ�״̬
		@param		[����]��	hItem��		��ǰ�ļ��ľ��
		@return		ITEM_CHECK_STATE��	��״̬
		@author		������
		@date		2014.10.15
		@remark
		*///}========================================================
		static ITEM_CHECK_STATE ItemCheckState(const PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		����ĳ��Item�ĸ��ڵ�
		@param		[����]��	hItem��		 ���
		@return		PSIHANDLE��            ���ڵ�Handle
		@author		������
		@date		2014.10.15
		@remark
		*///}========================================================
		static PSIHANDLE GetItemParent(const PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		�ж�ĳ��Item�Ƿ����ļ�
		@param		[����]��	hItem��		 ���
		@return		bool��            �Ƿ���true
		@author		������
		@date		2014.10.15
		@remark
		*///}========================================================
		static bool IsFolder(PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		����item��״̬
		@param		[����]��	hItem��		 ���
		@param		[����]��	bCheck��		 �Ƿ��
		@return		void
		@author		������
		@date		2014.10.15
		@remark	    ������û�ı������ӽڵ�͸��ڵ��״̬
		*///}========================================================
		static void SetItemChecked(PSIHANDLE hItem, bool bCheck);

		//���ö��item��״̬����ЩitemsӦ����һ�����ڵ���
		//������ÿ��item������SetItemCheckedҪ��
		static void SetItemsChecked(const std::vector<PSIHANDLE>& items, bool bCheck);

		//===========================================================
		// brief :		��ȡhItem��Ӧ�Ľڵ�����ѡ�е��ӽڵ����
		// Returns:		int		��ѡ�е��Լ������
		// Parameter:	hItem	����ȡ��Item�ڵ�
		// Author:    	jiaowei
		// DATE:      	2014.10.25 15:04:36
		//===========================================================
		static int GetSubCheckedCount(PSIHANDLE hItem);

		//===========================================================
		// brief :		��ȡhParent��Ӧ�ӽڵ�PSIHANDLE
		// Returns:		PSIHANDLE		�ӽڵ�PSIHANDLE
		// Parameter:	hParent		����ȡPSIHANDLE�ĸ��ڵ�
		//				index		���ڵ��Ӧ������
		// Author:    	jiaowei
		// DATE:      	2014.10.25 15:45:22
		//===========================================================
		static PSIHANDLE GetSubItemHandle(PSIHANDLE hParent, unsigned int index);

		/*{=====================================================*//**
		@brief		����ĳ��item������
		@param		[����]��	hItem��      ��ǰ���
		@return     std::wstring ��      item������
		@author		������
		@date		2014.10.27
		@remark		����hItem������
		*///}========================================================
		static const wchar_t* GetItemName(const PSIHANDLE pHItem);

		/*{=====================================================*//**
		@brief		����ĳ��item�ĵ�ȫ·��
		@param		[����]��	hItem��      ��ǰ���
		@return     std::wstring ��      item��ȫ·��
		@author		������
		@date		2014.10.27
		@remark		����hItem��ȫ·��
		*///}========================================================
		static std::wstring GetItemPath(PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		����ĳ��item����չ��
		@param		[����]��	hItem��      ��ǰ���
		@return     std::wstring ��      item����չ��
		@author		������
		@date		2014.10.27
		@remark		�����ļ�����չ������չ������"."
		*///}========================================================
		static const wchar_t* GetFileExtName(const PSIHANDLE phFile);

		//===========================================================
		// brief :		��ȡָ��Item��С
		// Returns:		unsigned __int64 Item����ļ��Ĵ�С
		// Parameter:	hItem��      ����ȡ��Item���
		// Author:    	jiaowei
		// DATE:      	2014.11.3 11:38:38
		//===========================================================
		static unsigned __int64 GetItemSize(PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		�жϴ����Ƿ�ΪSafeImager��ȡ֤U��
		@param		[����]��	hdevice��      ���̾��
		@return     bool                   �ǣ�true
		@author		������
		@date		2014.11.11
		*///}========================================================
		static bool IsSafeImagerDisk(PSIHANDLE hDevice);

		static bool IsSafeImagerVolume(PSIHANDLE hVolume);

		/*{=====================================================*//**
		@brief		�ж�ĳ��Item�Ƿ�Ϊɾ���Ľڵ�
		@param		[����]��	hItem��      ��ǰ���
		@return		bool��        ��ɾ����true
		@author		������
		@date		2014.11.12
		@remark		���ھ����̣�����false��
		*///}========================================================
		static bool IsItemDeleted(const PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		����ĳ��item�Ĵ���ʱ��
		@param		[����]��	hItem��   ��ǰ���
		@return     PPTIME��          ʱ��
		@author		������
		@date		2014.11.11
		@remark		ֻ���ļ���Ŀ¼��ʱ�����ԣ��������͵�Item�򷵻�NULL
		*///}========================================================
		static PPTIME GetItemCreateTime(const PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		����ĳ��item���޸�ʱ��
		@param		[����]��	hItem��   ��ǰ���
		@return     PPTIME��          ʱ��
		@author		������
		@date		2014.11.11
		@remark		ֻ���ļ���Ŀ¼��ʱ�����ԣ��������͵�Item�򷵻�NULL
		*///}========================================================
		static PPTIME GetItemModfiyTime(const PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		����ĳ��item��������ʱ��
		@param		[����]��	hItem��   ��ǰ���
		@return     PPTIME��          ʱ��
		@author		������
		@date		2014.11.11
		@remark		ֻ���ļ���Ŀ¼��ʱ�����ԣ��������͵�Item�򷵻�NULL
		*///}========================================================
		static PPTIME GetItemLastAccessTime(const PSIHANDLE hItiem);

		// ĳ���ڵ����Ƿ���Ŀ¼�ڵ�
		static bool CanExpand(PSIHANDLE pItem);
		static int GetItemLevel(PSIHANDLE pItem);

		/*{=====================================================*//**
		@brief		��������ӽڵ����Ŀ(���ݲ����ж��Ƿ�ݹ�)
		@param		[����]��	hItem��			���
		@param		[����]��	bRecursion��	�Ƿ�ݹ�
		@param		[����]��	expression: ���˱��ʽ
		@date		2018.7.30
		@remark	    ��ȵݹ���������ӽڵ����Ŀ
		*///}========================================================
		static void GetAllSubItems(PSIHANDLE hItem, std::vector<PSIHANDLE>& subItems,
			bool bRecursion = false, const std::wstring& expression = L"");

	private:
		/*{=====================================================*//**
		@brief		�ж��ӽڵ��Ƿ�ȫ��
		@param		[����]��	hItem��		 ���
		@return		bool��            �Ƿ���true
		@author		������
		@date		2014.10.15
		@remark	    û���ӽڵ��item��������Ĵ�״̬��UI�㲻Ӧ�õ��ô˺���
		*///========================================================
		static bool IsSubAllChecked(PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		�ݹ������ӽڵ��״̬
		@param		[����]��	hItem��		 ���
		@param		[����]��	bCheck��		 �Ƿ��
		@return		void
		@author		������
		@date		2014.10.15
		@remark	    �ݹ����������ӽڵ��״̬��UI�㲻Ӧ�õ��ô˺���
		*///========================================================
		static void SetSubAllChecked(PSIHANDLE hItem, bool bCheck);

		//===========================================================
		// brief :		�ж��ӽڵ��Ƿ�Ϊȫ��δ��
		// Returns:		bool	�Ƿ�Ϊȫδѡ
		// Parameter:	hItem��		 ���
		// Author:    	jiaowei
		// DATE:      	2014.10.22 17:25:32
		//===========================================================
		static bool IsSubAllUnChecked(PSIHANDLE hItem);

		/*{=====================================================*//**
		@brief		ͨ��·���õ��ļ���ľ��
		@param		[����]��hParent��		��Ŀ¼�ľ��
		@param		[����]��itemPath��	�ļ���·��
		@param		[����]: CaseSensitive         �Ƿ����ִ�Сд
		@return		PSIHANDLE��		�ļ���ľ��
		@author		������
		@date		2015.4.1
		@remark
		*///}========================================================
		static PSIHANDLE GetItemHandleRecursive(PSIHANDLE hParent,
			const std::wstring itemPath, bool CaseSensitive = false);

	private:
		static std::map<std::wstring, std::wstring> volumeMountPaths_;
	};
}
#endif
