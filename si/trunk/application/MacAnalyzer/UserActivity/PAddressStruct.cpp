// CPAddressStruct����ʵ���ļ�
//
#include "stdafx.h"
#include "PAddressStruct.h"

CPAddressStruct::CPAddressStruct(int nID)
{
    m_nID = nID;
}


CPAddressStruct::CPAddressStruct()
{
    m_nID = 0;
}


CPAddressStruct::~CPAddressStruct()
{
}


/**
 * @brief	��ȡ����ʶ��ID
 * @param	[in]:	void
 * @return	int
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
 int CPAddressStruct::GetID()
 {
	 return m_nID;
 }


/**
 * @brief	��ȡ��ϵ�˵�����
 * @param	[in]:	void		
 * @return	const std::wstring &
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
 const std::wstring & CPAddressStruct::GetLastName() const
 {
     return m_wstrLastName;
 }


 /**
 * @brief	������ϵ�˵�����
 * @param	[in]:	wstrLastName	��ϵ�˵�����
 * @return	void
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
 void CPAddressStruct::SetLastName(const std::wstring & wstrLastName)
 {
     if (wstrLastName.empty())
     {
         return;
     }

     m_wstrLastName = wstrLastName;
 }


/**
 * @brief	��ȡ��ϵ�˵�����
 * @param	[in]:   void
 * @return	const std::wstring &
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
const std::wstring & CPAddressStruct::GetFirstName() const
{
    return m_wstrFirstName;
}


/**
 * @brief	������ϵ�˵�����
 * @param	[in]:   wstrFirstName
 * @return	void
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
void CPAddressStruct::SetFirstName(const std::wstring & wstrFirstName)
{
    if (wstrFirstName.empty())
    {
        return;
    }

    m_wstrFirstName = wstrFirstName;
}


/**
 * @brief	��ȡ��˾������
 * @param	[in]:   void
 * @return	const std::wstring & 
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
const std::wstring & CPAddressStruct::GetCompanyName() const
{
    return m_wstrCompany;
}


/**
 * @brief	���ù�˾������
 * @param	[in]:   wstrCompanyName
 * @return	void
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
void CPAddressStruct::SetCompanyName(const std::wstring & wstrCompanyName)
{
    if (wstrCompanyName.empty())
    {
        return;
    }

    m_wstrCompany = wstrCompanyName;
}


/**
 * @brief	��ȡ�绰�б�
 * @param	[in]:   void
 * @return	const std::multimap<PHONETYPE, std::wstring> &
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
const std::multimap<PHONETYPE, std::wstring> & CPAddressStruct::GetPhoneNumber() const
{
    return m_multimapPhone;
}


/**
 * @brief	���ӵ绰��¼
 * @param	[in]:   phoneType
 * @param   [in]:   wstrPhoneNumber
 * @return	void
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
void CPAddressStruct::AddPhone(PHONETYPE phoneType, const std::wstring & wstrPhoneNumber)
{
    if (wstrPhoneNumber.empty())
    {
        return;
    }

	m_multimapPhone.insert(std::make_pair(phoneType, wstrPhoneNumber));
}


/**
 * @brief	��ȡ�����ʼ��б�
 * @param	[in]:   void
 * @return	const std::multimap<ADDRTYPE, std::wstring> &
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
const std::multimap<ADDRTYPE, std::wstring> & CPAddressStruct::GetEmail() const
{
    return m_multimapEmail;
}


/**
 * @brief	���ӵ����ʼ���ַ
 * @param	[in]:   addrType
 * @param   [in]:   wstrEmail
 * @return	void
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
void CPAddressStruct::AddEmail(ADDRTYPE addrType, const std::wstring & wstrEmail)
{
    if (wstrEmail.empty())
    {
        return;
    }

	m_multimapEmail.insert(std::make_pair(addrType, wstrEmail));
}


/**
 * @brief	��ȡ������ϵ�˵������б�
 * @param	[in]:   void
 * @return	const std::vector<std::wstring> &
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
const std::vector<std::wstring> & CPAddressStruct::GetRelated() const
{
    return m_vecRelatedName;
}


/**
 * @brief	���ӻ�����ϵ������
 * @param	[in]:   wstrRelatedName
 * @return	void
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
 void CPAddressStruct::AddRelated(const std::wstring & wstrRelatedName)
 {
     if (wstrRelatedName.empty())
     {
         return;
     }

     m_vecRelatedName.push_back(wstrRelatedName);
 }


 /**
 * @brief	��ȡ��ʱͨѶ�˺��б�
 * @param	[in]:   void
 * @return	std::multimap<ADDRTYPE, std::wstring> &
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
const std::multimap<ADDRTYPE, std::wstring> & CPAddressStruct::GetIM() const
{
    return m_multimapIM;
}


/**
 * @brief	���Ӽ�ʱͨѶ�˺�
 * @param	[in]:   wstrIMName
 * @return	void
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
void CPAddressStruct::AddIM(ADDRTYPE addrType, const std::wstring & wstrIMName)
{
    if (wstrIMName.empty())
    {
        return;
    }

	m_multimapIM.insert(std::make_pair(addrType, wstrIMName));
}



/**
 * @brief	��ȡסַ�б�
 * @param	[in]:   void
 * @return	std::multimap<ADDRTYPE, ADDRESS> &
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
const std::multimap<ADDRTYPE, ADDRESS>& CPAddressStruct::GetAddress() const
{
    return m_multimapAddress;
}


/**
 * @brief	����סַ
 * @param   [in]:   addrType
 * @param	[in]:   wstrAddress
 * @return	void
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
void CPAddressStruct::AddAddress(ADDRTYPE addrType, ADDRESS address)
{
//    if (address.m_wstrStreet.empty())
//    {
//        return;
//    }

	m_multimapAddress.insert(std::make_pair(addrType, address));
}


/**
 * @brief	��ȡ��ע
 * @param	[in]:   void
 * @return	const std::wstring &
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
const std::wstring & CPAddressStruct::GetNote() const
{
    return m_wstrNote;
}


/**
 * @brief	���ñ�ע
 * @param	[in]:   wstrNote
 * @return	void
 * @author	nct(xgs@pansafe.com)
 * @date	2009.04.20
 */
void CPAddressStruct::SetNote(const std::wstring & wstrNote)
{
    if (wstrNote.empty())
    {
        return;
    }

    m_wstrNote = wstrNote;
}

