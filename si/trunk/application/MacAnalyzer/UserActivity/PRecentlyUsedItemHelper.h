#ifndef RECENTLYUSEDITEMHELPER_H
#define RECENTLYUSEDITEMHELPER_H


namespace macbasic {

    typedef struct RecentlyUsedItem
    {
        std::wstring m_wsName;			// 名称
        std::wstring m_wsAddress;       // 地址
    }RecentlyUsedItem;
    typedef std::map<std::wstring,std::vector<RecentlyUsedItem> >ALLRecentData ;


    class  RecentlyUsedItemhelper
    {

    public:

            RecentlyUsedItemhelper();
            ~RecentlyUsedItemhelper();
     public:
           bool GetRecentusedItemRecords(CPRecord* rootRecord);
           bool GetRecentlyUsedItemInformations(std::wstring path);
           void AnalysisArray(boost::any para);
    private:
        std::wstring m_wType;			// 最近使用项类型
        std::vector<RecentlyUsedItem>all_visitName; //最近使用项所有名称、地址
        ALLRecentData all_RecentlyUsedrecord;//最近使用项类型、所有名称
    };
}


#endif
