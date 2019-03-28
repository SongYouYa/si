#ifndef _SI_MAC_ADDRESSTASKHELPER_H
#define _SI_MAC_ADDRESSTASKHELPER_H

class CPAddressAnalyzer;

namespace macbasic {

    class AddressTaskHelper
    {
        public:

            AddressTaskHelper();

            ~AddressTaskHelper();

        public:

            /**
             *@brief    Task thread
             *@author   nct(xgs@pansafe.com)
             */
            bool GetAddressBookInformations(CPRecord* rootRecord);

            /**
             * @brief
             * @param	[in]:	pDelegate
             * @return
             * @author	jiaowei
             * @date	2009.04.20
             */
            bool GetAddressBookRecords(CPRecord* rootRecord);
        private:

            /**
             * @brief   analysis addressbook instance
             */
            bool analysisAddress(const std::wstring & qstrAddress, CPRecord* rootRecord);

            /**
             * @brief   report output
             */
            bool GetRecord(CPAddressAnalyzer & reportAnalyzer,CPRecord* rootRecord);

            /**
             * @brief   find all addressbook file
             * @param   [in]    QString
             * @param   [in]    std::vector<QString> &
             * @return  void
             */
//            bool iterateAll(std::vector<siutil::CFileExport* > &vecExportFiles, const std::wstring &qstrDir,
//                            std::vector<std::wstring> & vecFiles, task::TaskStatusCtrl *pCtrl);

        private:

    };
}


#endif
