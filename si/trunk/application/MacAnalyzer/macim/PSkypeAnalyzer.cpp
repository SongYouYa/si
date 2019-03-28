#include "stdafx.h"
#include "PSkypeAnalyzer.h"
#include <QXmlStreamReader>


namespace macbasic {

CPSkypeAnalyzer::CPSkypeAnalyzer()
    : m_pDB(NULL)
{
}

bool CPSkypeAnalyzer::GetSkypeUserInfomation(const std::wstring &wstrDBName, std::vector<SkypeUserRecord> & vecUserRecords)
{
    if (wstrDBName.empty()) {
        return false;
    }
    sqlite3 * db = NULL;
    std::string szPath = CPCodeChanger::UnicodeToUTF8(wstrDBName.c_str());

    std::string strSql = "select liveid_membername, fullname, displayname, gender, birthday, country, province, city, phone_home, phone_office, phone_mobile, emails, homepage, about from Accounts";
    try {
        if (sqlite3_open(szPath.c_str(), &db)) {
            sqlite3_close(db);
            return false;
        }
        if (!db) {
            return false;
        }
        sqlite3_stmt * pStmt = NULL;
        if (SQLITE_OK != sqlite3_prepare(db, strSql.c_str(), -1,  &pStmt, NULL)) {
            return false;
        }

        while (SQLITE_ROW == sqlite3_step(pStmt)) {
            SkypeUserRecord userRecored;
            std::string strText;
            if (NULL != (const char*)sqlite3_column_text(pStmt, 0)) {
                strText = (const char*)sqlite3_column_text(pStmt, 0);
                userRecored.m_strUserName       = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 1)){
                strText = (const char*)sqlite3_column_text(pStmt, 1);
                userRecored.m_strSignName       = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 2)) {
                strText = (const char*)sqlite3_column_text(pStmt, 2);
                userRecored.m_strDisplayName    = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 3)) {
                strText = (const char*)sqlite3_column_text(pStmt, 3);
                std::wstring strGender          = CPCodeChanger::UTF8ToUnicode(strText.c_str());
                userRecored.m_strSex            = GetUserGender(strGender);
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 4)) {
                strText = (const char*)sqlite3_column_text(pStmt, 4);
                userRecored.m_strbirthday    = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 5)) {
                strText = (const char*)sqlite3_column_text(pStmt, 5);
                userRecored.m_strCountryCode    = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 6)) {
                strText = (const char*)sqlite3_column_text(pStmt, 6);
                userRecored.m_strProvince       = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 7)) {
                strText = (const char*)sqlite3_column_text(pStmt, 7);
                userRecored.m_strCity           = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 8)) {
                strText = (const char*)sqlite3_column_text(pStmt, 8);
                userRecored.m_strPhoneNumber    = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 9)) {
                strText = (const char*)sqlite3_column_text(pStmt, 9);
                userRecored.m_strOfficeNumber   = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 10)) {
                strText = (const char*)sqlite3_column_text(pStmt, 10);
                userRecored.m_strMobileNumber   = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 11)) {
                strText = (const char*)sqlite3_column_text(pStmt, 11);
                userRecored.m_strEmail          = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 12)) {
                strText = (const char*)sqlite3_column_text(pStmt, 12);
                userRecored.m_strMainHome       = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 13)) {
                strText = (const char*)sqlite3_column_text(pStmt, 13);
                userRecored.m_strAbout          = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            strText.clear();
            vecUserRecords.push_back(userRecored);
        }
        sqlite3_finalize(pStmt);
    }
    catch (...) {
        return false;
    }
    return true;
}

bool CPSkypeAnalyzer::GetSkypeContactInformation(const std::wstring &wstrDBName, std::vector<SkypeContactRecord> & vecContactRecords)
{
    if (wstrDBName.empty()) {
        return false;
    }
    sqlite3 * db = NULL;
    std::string szPath = CPCodeChanger::UnicodeToUTF8(wstrDBName.c_str());

    std::string strSql = "select skypename, fullname, displayname, gender, birthday, country, province, city, phone_home, phone_office, phone_mobile, emails, homepage, about, isauthorized from Contacts";
    try {
        if (sqlite3_open(szPath.c_str(), &db)) {
            sqlite3_close(db);
            return false;
        }
        if (!db) {
            return false;
        }

        sqlite3_stmt * stmt = NULL;
        std::string strBelong;
        if (SQLITE_OK != sqlite3_prepare(db, "select liveid_membername from Accounts", -1,  &stmt, NULL)) {
            return false;
        }
        if (SQLITE_ROW == sqlite3_step(stmt)) {
            const char* pBelong = (const char*)sqlite3_column_text(stmt, 0);
            if (pBelong) {
                strBelong = pBelong;
            }
        }

        sqlite3_stmt * pStmt = NULL;
        if (SQLITE_OK != sqlite3_prepare(db, strSql.c_str(), -1,  &pStmt, NULL)) {
            return false;
        }

        while (SQLITE_ROW == sqlite3_step(pStmt)) {
            SkypeContactRecord contactRecord;
            const char* pText = (const char*)sqlite3_column_text(pStmt, 14);
            if (!pText){
                continue;
            }
            std::string strText = pText;
            if (1 != std::atoi(strText.c_str())) {
                continue;
            }

            if (NULL != (const char*)sqlite3_column_text(pStmt, 0)) {
                strText = (const char*)sqlite3_column_text(pStmt, 0);
                contactRecord.m_accountInfo.m_strUserName       = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 1)) {
                strText = (const char*)sqlite3_column_text(pStmt, 1);
                contactRecord.m_accountInfo.m_strSignName       = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }
            if (NULL != (const char*)sqlite3_column_text(pStmt, 2)) {
                strText = (const char*)sqlite3_column_text(pStmt, 2);
                contactRecord.m_accountInfo.m_strDisplayName    = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }

            if (NULL != (const char*)sqlite3_column_text(pStmt, 3)) {
                strText = (const char*)sqlite3_column_text(pStmt, 3);
                std::wstring strGender          = CPCodeChanger::UTF8ToUnicode(strText.c_str());
                contactRecord.m_accountInfo.m_strSex            = GetUserGender(strGender);
            }

            if (NULL != (const char*)sqlite3_column_text(pStmt, 4)) {
                strText = (const char*)sqlite3_column_text(pStmt, 4);
                contactRecord.m_accountInfo.m_strbirthday    = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }

            if (NULL != (const char*)sqlite3_column_text(pStmt, 5)) {
                strText = (const char*)sqlite3_column_text(pStmt, 5);
                contactRecord.m_accountInfo.m_strCountryCode    = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }

            if (NULL != (const char*)sqlite3_column_text(pStmt, 6)) {
                strText = (const char*)sqlite3_column_text(pStmt, 6);
                contactRecord.m_accountInfo.m_strProvince       = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }

            if (NULL != (const char*)sqlite3_column_text(pStmt, 7)) {
                strText = (const char*)sqlite3_column_text(pStmt, 7);
                contactRecord.m_accountInfo.m_strCity           = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }

            if (NULL != (const char*)sqlite3_column_text(pStmt, 8)) {
                strText = (const char*)sqlite3_column_text(pStmt, 8);
                contactRecord.m_accountInfo.m_strPhoneNumber    = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }

            if (NULL != (const char*)sqlite3_column_text(pStmt, 9)) {
                strText = (const char*)sqlite3_column_text(pStmt, 9);
                contactRecord.m_accountInfo.m_strOfficeNumber   = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }

            if (NULL != (const char*)sqlite3_column_text(pStmt, 10)) {
                strText = (const char*)sqlite3_column_text(pStmt, 10);
                contactRecord.m_accountInfo.m_strMobileNumber   = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }

            if (NULL != (const char*)sqlite3_column_text(pStmt, 11)) {
                strText = (const char*)sqlite3_column_text(pStmt, 11);
                contactRecord.m_accountInfo.m_strEmail          = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }

            if (NULL != (const char*)sqlite3_column_text(pStmt, 12)) {
                strText = (const char*)sqlite3_column_text(pStmt, 12);
                contactRecord.m_accountInfo.m_strMainHome       = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }

            if (NULL != (const char*)sqlite3_column_text(pStmt, 13)) {
                strText = (const char*)sqlite3_column_text(pStmt, 13);
                contactRecord.m_accountInfo.m_strAbout          = CPCodeChanger::UTF8ToUnicode(strText.c_str());
            }

            contactRecord.m_strBelong = CPCodeChanger::UTF8ToUnicode(strBelong.c_str());

            strText.clear();
            vecContactRecords.push_back(contactRecord);
        }
        sqlite3_finalize(pStmt);
    }
    catch (...) {
        return false;
    }
    return true;
}

bool CPSkypeAnalyzer::GetSkypeMSgInformation(const std::wstring & wstrDBName, std::vector<SkypeMsgRecord> & vecMsgRecords)
{
    if (wstrDBName.empty()) {
        return false;
    }

    std::string szPath = CPCodeChanger::UnicodeToUTF8(wstrDBName.c_str());
    try {
        if (sqlite3_open(szPath.c_str(), &m_pDB)) {
            sqlite3_close(m_pDB);
            return false;
        }
        if (!m_pDB) {
            return false;
        }
    }
    catch (...) {
        return false;
    }

    GetAccountInfo();
    if (!GetChatMsgInformation(vecMsgRecords))
    {
        sqlite3_close(m_pDB);
        return false;
    }
    if (!GetCallMsgInformation(vecMsgRecords))
    {
        sqlite3_close(m_pDB);
        return false;
    }
    if (!GetTransferMsgInfortion(vecMsgRecords))
    {
        sqlite3_close(m_pDB);
        return false;
    }
    sqlite3_close(m_pDB);
    return true;
}

bool CPSkypeAnalyzer::GetChatMsgInformation(std::vector<SkypeMsgRecord> & vecMsgRecords)
{
    int result;
    char* errmsg = NULL;
    char** dbResult;
    int nRow;
    int nColumn;
    uint64 index;
    try {
        std::string strSql = "select chatname, author, from_dispname, timestamp, body_xml from Messages";
        result = sqlite3_get_table(m_pDB, strSql.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
        if (SQLITE_OK != result)
        {
            sqlite3_close(m_pDB);
            return false;
        }
    }
    catch (...)
    {
        return false;
    }
    index = nColumn;
    for (int i=0; i< nRow; ++i)
    {
        SkypeMsgRecord msgRecord;
        if (m_strUserDispName.empty()) {
            msgRecord.m_strUserName = m_strUserAccount;
        }
        else {
            msgRecord.m_strUserName = m_strUserDispName + L"(" + m_strUserAccount + L")";
        }
        std::wstring strChatName    = CPCodeChanger::UTF8ToUnicode(dbResult[index]);
        std::wstring strAuthor      = CPCodeChanger::UTF8ToUnicode(dbResult[index + 1]);
        std::wstring strFromName    = CPCodeChanger::UTF8ToUnicode(dbResult[index + 2]);
        char* strBeginTime          = dbResult[index + 3];
        std::string strContent      = QString(dbResult[index + 4]).toStdString();

        if (m_strUserName == strAuthor) {
            msgRecord.m_strSender = msgRecord.m_strUserName;

            if (strChatName.substr(0, strAuthor.size() + 3) == L"#" + strAuthor + L"/$") {
                size_t posB = strChatName.find(L"/$");
                size_t posE = strChatName.find(L";");
                msgRecord.m_strReceiver = strChatName.substr(posB + 2, posE - posB - 2);
            }
            else {
                msgRecord.m_strReceiver = strChatName;
            }
        }
        else {
            msgRecord.m_strSender = strFromName + L"(" + strAuthor + L")";
            msgRecord.m_strReceiver = msgRecord.m_strUserName;
        }


        if (!(strBeginTime == NULL))
        {
            long iBeginTime = 0;
            iBeginTime = atol(strBeginTime);
            LToTime(iBeginTime, msgRecord.m_iBeginTime);
        }

        AnalyzerContent(strContent, msgRecord.m_strContent);
        msgRecord.m_iMsgType = Skype_ChatMsg;
        if (!m_bPart) { // 由于Message表中的语音和CallMembers中的记录重复，暂时不显示
            vecMsgRecords.push_back(msgRecord);
        }
        index += nColumn;
    }

    sqlite3_free_table(dbResult);
    return true;
}
bool CPSkypeAnalyzer::GetCallMsgInformation(std::vector<SkypeMsgRecord> & vecMsgRecords)
{
    int result;
    char* errmsg = NULL;
    char** dbResult;
    int nRow;
    int nColumn;
    uint64 index;
    try {
        std::string strSql = "select type, identity, dispname, call_duration, creation_timestamp from CallMembers";
        result = sqlite3_get_table(m_pDB, strSql.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
        if (SQLITE_OK != result)
        {
            sqlite3_close(m_pDB);
            return false;
        }
    }

    catch (...)
    {
        return false;
    }

    index = nColumn;
    for (int i=0; i< nRow; ++i)
    {
        SkypeMsgRecord msgRecord;
        if (m_strUserDispName.empty()) {
            msgRecord.m_strUserName = m_strUserAccount;
        }
        else {
            msgRecord.m_strUserName = m_strUserDispName + L"(" + m_strUserAccount + L")";
        }

        std::wstring strType        = CPCodeChanger::UTF8ToUnicode(dbResult[index]);
        std::wstring stridentity    = CPCodeChanger::UTF8ToUnicode(dbResult[index + 1]);
        std::wstring strDisplayName = CPCodeChanger::UTF8ToUnicode(dbResult[index + 2]);
        char* strCallDuration       = dbResult[index + 3];
        char* strBeginTime          = dbResult[index + 4];

        long iCallDuration = 0;
        long iBeginTime = 0;
        if (!(strCallDuration == NULL))
        {
            iCallDuration = atol(strCallDuration);
        }

        if (!(strBeginTime == NULL))
        {
            iBeginTime = atol(strBeginTime);
        }

        bool bSengTag = (strType == _T("2")) ? true : false;
        if (bSengTag)
        {
            msgRecord.m_strSender = msgRecord.m_strUserName;
            msgRecord.m_strReceiver = strDisplayName + L"(" + stridentity + L")";
        }
        else
        {
            msgRecord.m_strSender = strDisplayName +L"(" + stridentity + L")";
            msgRecord.m_strReceiver = msgRecord.m_strUserName;;
        }
        LToTime(iBeginTime, msgRecord.m_iBeginTime);
        if (0 != iCallDuration) {
            LToTime(iBeginTime + iCallDuration, msgRecord.m_iEndTime);
        }

        msgRecord.m_iMsgType = Skype_CallMsg;
        vecMsgRecords.push_back(msgRecord);
        index += nColumn;
    }
    sqlite3_free_table(dbResult);
    return true;
}
bool CPSkypeAnalyzer::GetTransferMsgInfortion(std::vector<SkypeMsgRecord> & vecMsgRecords)
{
    int result;
    char* errmsg = NULL;
    char** dbResult;
    int nRow;
    int nColumn;
    uint64 index;
    try {
        std::string strSql = "select type, partner_handle, partner_dispname, starttime, finishtime, filepath, filename, filesize, offer_send_list from Transfers";
        result = sqlite3_get_table(m_pDB, strSql.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
        if (SQLITE_OK != result)
        {
            sqlite3_close(m_pDB);
            return false;
        }
    }
    catch (...)
    {
        return false;
    }
    index = nColumn;
    for (int i=0; i< nRow; ++i)
    {
        SkypeMsgRecord msgRecord;
        if (m_strUserDispName.empty()) {
            msgRecord.m_strUserName = m_strUserAccount;
        }
        else {
            msgRecord.m_strUserName = m_strUserDispName + L"(" + m_strUserAccount + L")";
        }
        std::wstring strType        = CPCodeChanger::UTF8ToUnicode(dbResult[index]);
        std::wstring strHandle      = CPCodeChanger::UTF8ToUnicode(dbResult[index + 1]);
        std::wstring strDispname    = CPCodeChanger::UTF8ToUnicode(dbResult[index + 2]);
        char* strBeginTime          = dbResult[index + 3];
        char* strEndTime            = dbResult[index + 4];
        msgRecord.m_strFilePath     = CPCodeChanger::UTF8ToUnicode(dbResult[index + 5]);
        msgRecord.m_strFileName     = CPCodeChanger::UTF8ToUnicode(dbResult[index + 6]);
        msgRecord.m_strFileSize     = CPCodeChanger::UTF8ToUnicode(dbResult[index + 7]);
        std::wstring strSendList    = CPCodeChanger::UTF8ToUnicode(dbResult[index + 8]);

        long iBeginTime = 0;
        if (!(strBeginTime == NULL))
        {
            iBeginTime = atol(strBeginTime);
        }
        long iEndTime = 0;
        if (!(strEndTime == NULL))
        {
            iEndTime = atol(strEndTime);
        }
        LToTime(iBeginTime, msgRecord.m_iBeginTime);
        if (0 != iEndTime) {
            LToTime(iEndTime, msgRecord.m_iEndTime);
        }

        bool bType = (strType == _T("2")) ? true : false;
        if (bType) {
            if (strHandle == m_strUserName) {
                msgRecord.m_strSender = msgRecord.m_strUserName;
                std::wstring strOther = L"";
                if (!strSendList.empty()) {
                    size_t pos = strSendList.find(L" ");
                    if (pos != std::wstring::npos) {
                        if (strHandle == strSendList.substr(0, pos)) {
                            strOther = strSendList.substr(pos + 1);
                        }
                        else if (strHandle == strSendList.substr(pos + 1)) {
                            strOther = strSendList.substr(0, pos);
                        }
                        else {
                            strOther = strSendList;
                        }
                    }
                }
                msgRecord.m_strReceiver = strOther;
            }
            else {
                msgRecord.m_strSender = strDispname + L"(" + strHandle + L")";
                msgRecord.m_strReceiver = msgRecord.m_strUserName;
            }
        }
        else {
            if (strHandle == m_strUserName) {
                msgRecord.m_strReceiver = msgRecord.m_strUserName;
                std::wstring strOther = L"";
                if (!strSendList.empty()) {
                    size_t pos = strSendList.find(L" ");
                    if (pos != std::wstring::npos) {
                        if (strHandle == strSendList.substr(0, pos)) {
                            strOther = strSendList.substr(pos + 1);
                        }
                        else if (strHandle == strSendList.substr(pos + 1)) {
                            strOther = strSendList.substr(0, pos);
                        }
                        else {
                            strOther = strSendList;
                        }
                    }
                }
                msgRecord.m_strSender = strOther;
            }
            else {
                msgRecord.m_strReceiver = strDispname + L"(" + strHandle + L")";
                msgRecord.m_strSender = msgRecord.m_strUserName;
            }
        }

        msgRecord.m_iMsgType = Skype_TransferMsg;
        vecMsgRecords.push_back(msgRecord);
        index += nColumn;
    }
    sqlite3_free_table(dbResult);
    return true;
}

bool CPSkypeAnalyzer::AnalyzerContent(std::string &strContent, std::wstring &wstrMsg)
{
    try {
        QString strText;
        QXmlStreamReader reader(QString::fromStdString(strContent));

        //   reader.readNext();
        strText.clear();
        bool part = false;
        m_bPart = false;
        while(!reader.atEnd()) {
            if (reader.isStartElement()) {
                // 发送失败 + 内容
                if ("sms" == reader.name()) {
                    strText += "[失败]: " + reader.attributes().value("alt").toString();
                }
                // 表情
                else if ("ss" == reader.name()) {
                    strText += "[表情]: " + reader.attributes().value("type").toString();
                }
                // 联系人
                else if ("contacts" == reader.name()) {
                    strText += "[联系人]: ";
                }
                // URI
                else if ("URIObject" == reader.name()) {
                    if ("Picture.1" == reader.attributes().value("type")
                            || reader.attributes().value("type").contains("picure", Qt::CaseInsensitive)) {
                        strText += "[图片]: " + reader.attributes().value("uri").toString();
                    }
                }
                // 语音
                else if ("partlist" == reader.name()) {
                    if ("started" == reader.attributes().value("type")
                            || "ended" == reader.attributes().value("type")) {
                        strText += "[语音]";
                        part = true;
                        m_bPart = true;
                    }
                }
                // 文件 + name + size
                else if ("files" == reader.name()) {
                    strText += "[文件]: ";
                }

                // URI
                else if ("OriginalName" == reader.name()) {
                    strText += QString(" ") + reader.attributes().value("v").toString();
                }
                // 联系人
                if ("c" == reader.name()) {
                    strText += reader.attributes().value("s");
                }
                // 语音
                else if ("duration" == reader.name()) {
                    if (part) {
                        strText += ": " + reader.readElementText();
                        part = false;
                    }
                }
                // 文件
                else if ("file" == reader.name()) {
                    strText = strText + reader.readElementText() + " " + reader.attributes().value("size").toString();
                }
            }
            reader.readNext();
        }
        if (strText.isEmpty()) {
            wstrMsg = CPCodeChanger::UTF8ToUnicode(strContent.c_str());
        }
        else {
            wstrMsg = strText.toStdWString();
        }
    }
    catch (...) {
        wstrMsg = CPCodeChanger::UTF8ToUnicode(strContent.c_str());
        return false;
    }
    return true;
}

std::wstring CPSkypeAnalyzer::GetUserGender(const std::wstring &strCode)
{
    std::wstring strGender = L"不公开";
    if (strCode == L"1")
    {
        strGender = L"男";
    }
    else if (strCode == L"2")
    {
        strGender = L"女";
    }
    return strGender;

}

void CPSkypeAnalyzer::GetAccountInfo()
{
    int result;
    char* errmsg = NULL;
    char** dbResult;
    int nRow;
    int nColumn;
    uint64 index;
    try{
        std::string strSql = "select liveid_membername, skypename, fullname from Accounts";
        result = sqlite3_get_table(m_pDB, strSql.c_str(), &dbResult, &nRow, &nColumn, &errmsg);
        if (SQLITE_OK != result)
        {
            sqlite3_close(m_pDB);
            return;
        }
        index = nColumn;
        if (nRow > 0)
        {
            m_strUserAccount    = CPCodeChanger::UTF8ToUnicode(dbResult[index]);
            m_strUserName       = CPCodeChanger::UTF8ToUnicode(dbResult[index + 1]);
            m_strUserDispName   = CPCodeChanger::UTF8ToUnicode(dbResult[index + 2]);
        }
        sqlite3_free_table(dbResult);
    }
    catch(...)
    {
        return;
    }

}

void CPSkypeAnalyzer::LToTime(long ltime, CPTime & time)
{
    time_t ttTime = ltime;
    time = CPTime::UTC2LocalTime(ttTime, siutil::MacCommonHelper::GetTimezoneRegion());
    return;
}
}

