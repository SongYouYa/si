#ifndef MAC_SKYPEHELPER_H
#define MAC_SKYPEHELPER_H
    namespace macbasic {

    class  CPSkypeHelper
    {
    public:
        CPSkypeHelper();
        ~CPSkypeHelper();
        //===========================================================
        // brief :		获取Skype账号
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
        //void GetSkypeAccount(CPRecord *rootRecord);

        //===========================================================
        // brief :		获取Skype好友信息
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
        //void GetSkypeFriends(CPRecord *rootRecord);

        //===========================================================
        // brief :		获取Skype聊天信息
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
        //void GetSkypeMsgs(CPRecord *rootRecord);
   public:
        //===========================================================
        // brief :		具体解析账号及好友
        // Returns:
        // Parameter:	type	用户类型，账号或者好友
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
       static bool AnalyzerSkypeAccount(CPRecord *rootRecord);
       static bool AnalyzerSkypeContact(CPRecord *rootRecord);
        //===========================================================
        // brief :		具体解析消息记录
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
        static bool AnalyzerSkypeMsgs(CPRecord *rootRecord);

        //===========================================================
        // brief :		获取所有文件
        // Returns:
        // Parameter:	vecAllFiles	所有文件
        //				exportFiles	导出文件路径
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
        static bool GetAllFiles(std::vector<std::wstring> &vecAllFiles,
                         std::vector<siutil::CPFileExport*> &exportFiles
                         );

        //===========================================================
        // brief :		获取某个目录所有文件
        // Returns:
        // Parameter:	vecFiles		待提取的文件
        //				vecExportFiles	导出文件路径
        //				strDir			指定目录
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
        static bool iterateAll(std::vector<siutil::CPFileExport* > &vecExportFiles,
                                           const std::wstring &strDir,
                                           std::vector<std::wstring> & vecFiles);

    };
}


#endif // SKYPEHELPER_H
