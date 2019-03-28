#ifndef MAC_SKYPEANALYZER_H
#define MAC_SKYPEANALYZER_H
extern "C" {
#include"sqlite3.h"
}

#include "macim/IMStruct.h"
#include <boost/thread/mutex.hpp>

namespace macbasic {

class CPSkypeAnalyzer
{
public:
    CPSkypeAnalyzer();
    //===========================================================
    // brief :		解析数据库中账号数据
    // Returns:
    // Parameter:	wstrDBName		数据库名
    //				vecUserRecords	输出参数，获取到的记录
    //				type			类型
    // Author:    	jiaowei
    // DATE:      	2015.8.10 17:25:38
    //===========================================================
    bool GetSkypeUserInfomation(const std::wstring & wstrDBName, std::vector<SkypeUserRecord> & vecUserRecords);
    bool GetSkypeContactInformation(const std::wstring & wstrDBName, std::vector<SkypeContactRecord> & vecContactRecords);

    //===========================================================
    // brief :		解析数据库中消息数据
    // Returns:
    // Parameter:	wstrDBName		数据库名
    //				vecMsgRecords	输出参数，获取到的记录
    // Author:    	jiaowei
    // DATE:      	2015.8.10 17:25:38
    //===========================================================
    bool GetSkypeMSgInformation(const std::wstring & wstrDBName, std::vector<SkypeMsgRecord> & vecMsgRecords);
private:

    //分别解析数据库中消息，语音及文件传输数据
    bool GetChatMsgInformation(std::vector<SkypeMsgRecord> & vecMsgRecords);
    bool GetCallMsgInformation(std::vector<SkypeMsgRecord> & vecMsgRecords);
    bool GetTransferMsgInfortion(std::vector<SkypeMsgRecord> & vecMsgRecords);

    bool AnalyzerContent(std::string & strContent, std::wstring & wstrMsg);
    //===========================================================
    // brief :		获取用户性别
    // Returns:
    // Parameter:	strCode 性别代码
    // Author:    	jiaowei
    // DATE:      	2015.8.10 17:25:38
    //===========================================================
    std::wstring GetUserGender(const std::wstring &strCode);

    //===========================================================
    // brief :		获取账号相关信息，即用户名及显示名
    // Returns:
    // Parameter:
    // Author:    	jiaowei
    // DATE:      	2015.8.10 17:25:38
    //===========================================================
    void GetAccountInfo();

    void LToTime(long ltime, CPTime & time);
private:
    bool m_bPart; // Messages表中的语音消息标记
    sqlite3 * m_pDB;
    std::wstring m_strUserAccount;
    std::wstring m_strUserName;
    std::wstring m_strUserDispName;
};
}
#endif // SKYPEANALYZER_H
