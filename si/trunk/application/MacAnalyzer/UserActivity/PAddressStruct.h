#ifndef _ADDRESSSTRUCT_HEADER_
#define _ADDRESSSTRUCT_HEADER_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * �绰���� eg: ��ͥ�绰, �����绰
 */
enum PHONETYPE
{
	PHONE_TYPE_WORK		= 0x00,
	PHONE_TYPE_HOME		= 0x01,
	PHONE_TYPE_MOBLIE	= 0x02,
	PHONE_TYPE_MAIN		= 0x03,
	PHONE_TYPE_HOMEFAX	= 0x04,
	PHONE_TYPE_WORKFAX	= 0x05,
	PHONE_TYPE_CALL		= 0x06,
	PHONE_TYPE_OTHER	= 0x07,
	PHONE_TYPE_DEFINE	= 0x08,
};


/*
 * ��ͥסַ����
 */
enum ADDRTYPE
{
	ADDR_TYPE_WORK		= 0x00,
	ADDR_TYPE_HOME		= 0x01,
	ADDR_TYPE_OTHER		= 0x02,
	ADDR_TYPE_DEFINE	= 0x03,
};


typedef struct ADDRESS
{
	std::wstring m_wstrStreet;			// �ֵ�
 //   std::wstring m_wstrAddress;         // ��ַ
	std::wstring m_wstrZipCode;         // �ʱ�
    std::wstring m_wstrCity;            // ����
    std::wstring m_wstrProvince;        // ʡ��
    std::wstring m_wstrCountry;         // ���һ����
}ADDRESS;

class CPAddressStruct
{
    public:
		CPAddressStruct(int nID);
        CPAddressStruct();
        ~CPAddressStruct();


	public:
		/**
		 * @brief	��ȡ���ʶ��ID
		 * @param	[in]:	void
		 * @return	int
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
		 int GetID(void);


		/**
		 * @brief	��ȡ��ϵ�˵�����
		 * @param	[in]:	void		
		 * @return	const std::wstring &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
		 const std::wstring & GetLastName() const;


		 /**
		 * @brief	������ϵ�˵�����
		 * @param	[in]:	wstrLastName	��ϵ�˵�����
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
		 void SetLastName(const std::wstring & wstrLastName);


		/**
		 * @brief	��ȡ��ϵ�˵�����
		 * @param	[in]:   void
		 * @return	const std::wstring &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::wstring & GetFirstName() const;


        /**
		 * @brief	������ϵ�˵�����
		 * @param	[in]:   wstrFirstName
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void SetFirstName(const std::wstring & wstrFirstName);


        /**
		 * @brief	��ȡ��˾������
		 * @param	[in]:   void
		 * @return	const std::wstring & 
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::wstring & GetCompanyName() const;


        /**
		 * @brief	���ù�˾������
		 * @param	[in]:   wstrCompanyName
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void SetCompanyName(const std::wstring & wstrCompanyName);


        /**
		 * @brief	��ȡ�绰�б�
		 * @param	[in]:   void
		 * @return	const std::multimap<PHONETYPE, std::wstring> &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::multimap<PHONETYPE, std::wstring> & GetPhoneNumber() const;


        /**
		 * @brief	��ӵ绰��¼
		 * @param	[in]:   phoneType
         * @param   [in]:   wstrPhoneNumber
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void AddPhone(PHONETYPE phoneType, const std::wstring & wstrPhoneNumber);


        /**
		 * @brief	��ȡ�����ʼ��б�
		 * @param	[in]:   void
		 * @return	const std::multimap<ADDRTYPE, std::wstring> &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::multimap<ADDRTYPE, std::wstring> & GetEmail() const;


        /**
		 * @brief	��ӵ����ʼ���ַ
		 * @param	[in]:   addrType
         * @param   [in]:   wstrEmail
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void AddEmail(ADDRTYPE addrType, const std::wstring & wstrEmail);


        /**
		 * @brief	��ȡ�����ϵ�˵������б�
		 * @param	[in]:   void
		 * @return	const std::vector<std::wstring> &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::vector<std::wstring> & GetRelated() const;


        /**
		 * @brief	��ӻ����ϵ������
		 * @param	[in]:   wstrRelatedName
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
         void AddRelated(const std::wstring & wstrRelatedName);


         /**
		 * @brief	��ȡ��ʱͨѶ�˺��б�
		 * @param	[in]:   void
		 * @return	std::multimap<ADDRTYPE, std::wstring> &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::multimap<ADDRTYPE, std::wstring> & GetIM() const;


        /**
		 * @brief	��Ӽ�ʱͨѶ�˺�
         * @param   [in]:   addrType
		 * @param	[in]:   wstrIMName
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void AddIM(ADDRTYPE addrType, const std::wstring & wstrIMName);


        /**
		 * @brief	��ȡסַ�б�
		 * @param	[in]:   void
		 * @return	const std::multimap<ADDRTYPE, ADDRESS> &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::multimap<ADDRTYPE, ADDRESS>& GetAddress() const;


        /**
		 * @brief	���סַ
         * @param   [in]:   addrType
		 * @param	[in]:   wstrAddress
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void AddAddress(ADDRTYPE addrType, ADDRESS address);


        /**
		 * @brief	��ȡ��ע
		 * @param	[in]:   void
		 * @return	const std::wstring &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::wstring & GetNote() const;


        /**
		 * @brief	���ñ�ע
		 * @param	[in]:   wstrNote
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void SetNote(const std::wstring & wstrNote);


    private:
		/*
		 * ʶ��ID
		 */
		int m_nID;
        /*
         * ��ϵ�˵�����
         */
        std::wstring m_wstrLastName;
        /*
         * ��ϵ�˵�����
         */
        std::wstring m_wstrFirstName;
        /*
         * ��ϵ�˹�˾������
         */
        std::wstring m_wstrCompany;
        /*
         * �绰�б�
         */
        std::multimap<PHONETYPE, std::wstring> m_multimapPhone;
        /*
         * �����ʼ��б�
         */
        std::multimap<ADDRTYPE, std::wstring> m_multimapEmail;
        /*
         * ��ع�ϵ�������б�
         */
        std::vector<std::wstring> m_vecRelatedName;
		//std::multimap<RELATEDTYPE, std::wstring> m_multimapRelated;
        /*
         * ��ʱͨѶ�˺��б�
         */
        std::multimap<ADDRTYPE, std::wstring> m_multimapIM;
        /*
         * סַ�б�
         */
        std::multimap<ADDRTYPE, ADDRESS> m_multimapAddress;
        /*
         * ��ע
         */
        std::wstring m_wstrNote;
};

bool operator== (const CPAddressStruct & obj1, const CPAddressStruct & obj2);

#ifdef __cplusplus
}
#endif

#endif

