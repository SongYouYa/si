#ifndef _ADDRESSANALYZER_HEADER_
#define _ADDRESSANALYZER_HEADER_

extern "C" { 
#include "sqlite3.h"
}

#include <cstdlib>
#include <cstring>

#include "PAddressStruct.h"

#ifdef __cplusplus
extern "C" {
#endif

  /*
#ifndef __ppc__
#ifdef __PPC__
#define __ppc__ __PPC__
#endif
#endif

#ifndef __ppc__
#ifdef __powerpc__
#define __ppc__ __powerpc__
#endif
#endif


  */
#define INIT_WCS_PTR(p) {if (NULL == p) {p = L"<null>"}}
namespace macbasic {
    class CPAddressAnalyzer
    {
        public:
            CPAddressAnalyzer();
            ~CPAddressAnalyzer();

        public:
            /**
             * @brief	�������ݿ��ļ�
             * @param	[in]:	wstrDBName			//���ݿ�������
             * @return	bool	true	���ʳɹ�	false	����ʧ��
             * @author	nct(xgs@pansafe.com)
             * @date	2009.04.20
             */
            bool Open(const std::wstring & wstrDBName);
            /**
             * @brief	�ر����ݿ�
             * @param	[in]:	void
             * @return	void
             * @author	nct(xgs@pansafe.com)
             * @date	2009.04.20
             */
            void Close();
            /**
             * @brief	��ʼ�������ݿ�
             * @param	[in]:	void
             * @return	void
             * @author	nct(xgs@pansafe.com)
             * @date	2009.04.20
             */
            void Analyzer();
            /**
             * @brief	ȡ�õ�һ���ϵ�˼�¼
             * @param	[in]:	void
             * @return	const CPAddressStruct *
             * @author	nct(xgs@pansafe.com)
             * @date	2009.04.20
             */
            const CPAddressStruct * GetFirstRecord();
            /**
             * @brief	ȡ����һ���ϵ�˼�¼
             * @param	[in]:	void
             * @return	const CPAddressStruct *
             * @author	nct(xgs@pansafe.com)
             * @date	2009.04.20
             */
            const CPAddressStruct * GetNextRecord();

        private:
            /**
            * @brief	��ȡĿ����Ŀ�Ķ���ָ��
            * @param	[in]:	void
            * @return	CPAddressStruct *
            * @author	nct(xgs@pansafe.com)
            * @date	2009.04.20
            */
            CPAddressStruct * GetDestAddress(int nID);


            /**
             * @brief	������¼
             * @param	[in]:	CPAddressStruct *
             * @return	void
             * @author	nct(xgs@pansafe.com)
             * @date	2009.04.20
             */
            void AnalyzerRecord(CPAddressStruct * pAddressInstance = NULL);
            /**
             * @brief	�����ʼ�
             * @param	[in]:	CPAddressStruct *
             * @return	void
             * @author	nct(xgs@pansafe.com)
             * @date	2009.04.20
             */
            void AnalyzerEmail(CPAddressStruct * pAddressInstance = NULL);
            /**
             * @brief	������ʱͨѶ
             * @param	[in]:	CPAddressStruct *
             * @return	void
             * @author	nct(xgs@pansafe.com)
             * @date	2009.04.20
             */
            void AnalyzerIM(CPAddressStruct * pAddressInstance = NULL);
            /**
             * @brief	������ע
             * @param	[in]:	CPAddressStruct *
             * @return	void
             * @author	nct(xgs@pansafe.com)
             * @date	2009.04.20
             */
            void AnalyzerNote(CPAddressStruct * pAddressInstance = NULL);
            /**
             * @brief	�����绰��
             * @param	[in]:	CPAddressStruct *
             * @return	void
             * @author	nct(xgs@pansafe.com)
             * @date	2009.04.20
             */
            void AnalyzerPhoneBook(CPAddressStruct * pAddressInstance = NULL);
            /**
             * @brief	����������ϵ
             * @param	[in]:	CPAddressStruct *
             * @return	void
             * @author	nct(xgs@pansafe.com)
             * @date	2009.04.20
             */
            void AnalyzerRelated(CPAddressStruct * pAddressInstance = NULL);
            /**
             * @brief	����סַ
             * @param	[in]:	CPAddressStruct *
             * @return	void
             * @author	nct(xgs@pansafe.com)
             * @date	2009.04.20
             */
            void AnalyzerPostedAddress(CPAddressStruct * pAddressInstance = NULL);

            /**
             * @brief   linux �� windows�Ĳ���������
             * @param   [in]:   ucs2 little char *
             * @return  std::wstring
             * @author  nct(xgs@pansafe.com)
             * @date    2009.10.27
             */
            std::wstring ucs2_ucs4(char * pointer, size_t size);


        private:

            sqlite3 * m_pDB;									//���ݿ�ָ��
            std::vector<CPAddressStruct *>::iterator m_itPos;	//��ǰ��¼������
            std::vector<CPAddressStruct *> m_vecList;			//�ϵ���б�
    };
    ADDRTYPE AddrTypeIdentfy(const std::wstring & wstrIdentfy);
    PHONETYPE PhoneTypeIdentfy(const std::wstring & wstrIdentfy);
}


#ifdef __cplusplus
}
#endif

#endif

