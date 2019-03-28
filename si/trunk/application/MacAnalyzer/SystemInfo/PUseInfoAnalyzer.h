#ifndef USEINFOANALYZER_H
#define USEINFOANALYZER_H

class CPTime;
class CPRecord;

namespace macbasic {

typedef struct
{
    std::wstring wstrGroupName;
    std::wstring wstrGID;

    std::list<std::wstring> listWstrUserName;

}GROUP;
typedef struct
{
    std::wstring wstrUserName;
    std::wstring wstrUID;
    std::wstring wstrUserType;
    std::wstring wstrGroupName;
    std::wstring wstrGID;
    std::wstring wstrHome;
    std::wstring wstrHomeLoc;
    std::wstring wstrRealName;
    CPTime timeCreated;
}USER;

class  CPUseInfoAnalyzer
{

public:
    CPUseInfoAnalyzer();
    ~CPUseInfoAnalyzer();

    bool GetUseInfo(CPRecord *rootRecord);
    bool ReadGroupPlistFile(std::wstring plistPath);
    bool ReadUserPlistFile(std::wstring plistPath,PSIHANDLE handle);
    /********************************************************
            * @brief	get group name by using user name
            * @param	[in] const std::wstring &    group id
            * @return	std::wstring                 group name
            * @author	Lin Min(linmin@pansafe.com)
            * @date	2010.01.25
            ********************************************************/
    std::wstring GetGroupName(const std::wstring & wstrUserName);
    /********************************************************
               * @brief	get user's type by user's name
               * @param	[in]  const std::wstring
               * @return      std::wstring
               * @author	Lin Min(linmin@pansafe.com)
               * @date	2010.01.25
               ********************************************************/
    std::wstring GetUserType(USER user);
    /********************************************************
               * @brief	get group stracture pointer by using group id
               * @param	[in] const std::wstring &    group id
               * @return	std::wstring                 group name
               * @author	Lin Min(linmin@pansafe.com)
               * @date	2010.01.25
               ********************************************************/
    GROUP * GetGroupByGID(const std::wstring wstrGID);
    /********************************************************
               * @brief	get user's type by user's name
               * @param	[in]  const std::wstring
               * @return      std::wstring
               * @author	Lin Min(linmin@pansafe.com)
               * @date	2010.01.25
               ********************************************************/
    bool findUserInGroup(const std::wstring wstrUserName, GROUP * pGroup);

    void WriteDataInTable(CPRecord* diskRecord,std::vector<USER> & vecUser);

private:
    std::vector<GROUP> m_vecGroup;
    std::vector<USER> m_vecUser;
};
}


#endif
