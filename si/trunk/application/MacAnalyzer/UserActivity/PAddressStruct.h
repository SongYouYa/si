#ifndef _ADDRESSSTRUCT_HEADER_
#define _ADDRESSSTRUCT_HEADER_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 电话类型 eg: 家庭电话, 工作电话
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
 * 家庭住址类型
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
	std::wstring m_wstrStreet;			// 街道
 //   std::wstring m_wstrAddress;         // 地址
	std::wstring m_wstrZipCode;         // 邮编
    std::wstring m_wstrCity;            // 城市
    std::wstring m_wstrProvince;        // 省份
    std::wstring m_wstrCountry;         // 国家或地区
}ADDRESS;

class CPAddressStruct
{
    public:
		CPAddressStruct(int nID);
        CPAddressStruct();
        ~CPAddressStruct();


	public:
		/**
		 * @brief	获取类的识别ID
		 * @param	[in]:	void
		 * @return	int
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
		 int GetID(void);


		/**
		 * @brief	获取联系人的姓氏
		 * @param	[in]:	void		
		 * @return	const std::wstring &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
		 const std::wstring & GetLastName() const;


		 /**
		 * @brief	设置联系人的姓氏
		 * @param	[in]:	wstrLastName	联系人的姓氏
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
		 void SetLastName(const std::wstring & wstrLastName);


		/**
		 * @brief	获取联系人的名字
		 * @param	[in]:   void
		 * @return	const std::wstring &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::wstring & GetFirstName() const;


        /**
		 * @brief	设置联系人的名字
		 * @param	[in]:   wstrFirstName
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void SetFirstName(const std::wstring & wstrFirstName);


        /**
		 * @brief	获取公司的名字
		 * @param	[in]:   void
		 * @return	const std::wstring & 
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::wstring & GetCompanyName() const;


        /**
		 * @brief	设置公司的名字
		 * @param	[in]:   wstrCompanyName
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void SetCompanyName(const std::wstring & wstrCompanyName);


        /**
		 * @brief	获取电话列表
		 * @param	[in]:   void
		 * @return	const std::multimap<PHONETYPE, std::wstring> &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::multimap<PHONETYPE, std::wstring> & GetPhoneNumber() const;


        /**
		 * @brief	添加电话记录
		 * @param	[in]:   phoneType
         * @param   [in]:   wstrPhoneNumber
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void AddPhone(PHONETYPE phoneType, const std::wstring & wstrPhoneNumber);


        /**
		 * @brief	获取电子邮件列表
		 * @param	[in]:   void
		 * @return	const std::multimap<ADDRTYPE, std::wstring> &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::multimap<ADDRTYPE, std::wstring> & GetEmail() const;


        /**
		 * @brief	添加电子邮件地址
		 * @param	[in]:   addrType
         * @param   [in]:   wstrEmail
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void AddEmail(ADDRTYPE addrType, const std::wstring & wstrEmail);


        /**
		 * @brief	获取相关联系人的名字列表
		 * @param	[in]:   void
		 * @return	const std::vector<std::wstring> &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::vector<std::wstring> & GetRelated() const;


        /**
		 * @brief	添加互相关系人名字
		 * @param	[in]:   wstrRelatedName
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
         void AddRelated(const std::wstring & wstrRelatedName);


         /**
		 * @brief	获取即时通讯账号列表
		 * @param	[in]:   void
		 * @return	std::multimap<ADDRTYPE, std::wstring> &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::multimap<ADDRTYPE, std::wstring> & GetIM() const;


        /**
		 * @brief	添加即时通讯账号
         * @param   [in]:   addrType
		 * @param	[in]:   wstrIMName
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void AddIM(ADDRTYPE addrType, const std::wstring & wstrIMName);


        /**
		 * @brief	获取住址列表
		 * @param	[in]:   void
		 * @return	const std::multimap<ADDRTYPE, ADDRESS> &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::multimap<ADDRTYPE, ADDRESS>& GetAddress() const;


        /**
		 * @brief	添加住址
         * @param   [in]:   addrType
		 * @param	[in]:   wstrAddress
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void AddAddress(ADDRTYPE addrType, ADDRESS address);


        /**
		 * @brief	获取备注
		 * @param	[in]:   void
		 * @return	const std::wstring &
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        const std::wstring & GetNote() const;


        /**
		 * @brief	设置备注
		 * @param	[in]:   wstrNote
		 * @return	void
		 * @author	nct(xgs@pansafe.com)
		 * @date	2009.04.20
		 */
        void SetNote(const std::wstring & wstrNote);


    private:
		/*
		 * 识别ID
		 */
		int m_nID;
        /*
         * 联系人的姓氏
         */
        std::wstring m_wstrLastName;
        /*
         * 联系人的名字
         */
        std::wstring m_wstrFirstName;
        /*
         * 联系人公司的名称
         */
        std::wstring m_wstrCompany;
        /*
         * 电话列表
         */
        std::multimap<PHONETYPE, std::wstring> m_multimapPhone;
        /*
         * 电子邮件列表
         */
        std::multimap<ADDRTYPE, std::wstring> m_multimapEmail;
        /*
         * 相关关系人名字列表
         */
        std::vector<std::wstring> m_vecRelatedName;
		//std::multimap<RELATEDTYPE, std::wstring> m_multimapRelated;
        /*
         * 即时通讯账号列表
         */
        std::multimap<ADDRTYPE, std::wstring> m_multimapIM;
        /*
         * 住址列表
         */
        std::multimap<ADDRTYPE, ADDRESS> m_multimapAddress;
        /*
         * 备注
         */
        std::wstring m_wstrNote;
};

bool operator== (const CPAddressStruct & obj1, const CPAddressStruct & obj2);

#ifdef __cplusplus
}
#endif

#endif

