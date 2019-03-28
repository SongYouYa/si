#ifndef TOOLS_FILEOPERATE_H
#define TOOLS_FILEOPERATE_H
namespace siutil {
    struct FileItemInfo
    {
        PSIHANDLE handle_;
        std::wstring name_;			//文件名
        uint64 size_;			//大小，已经转换成直观字符串，如4.5G
        std::wstring path_;			//路径
        CPTime createTime_;
        CPTime modifyTime_;
        CPTime accessTime_;
        std::wstring md5Hash_;
        FileItemInfo() : handle_(NULL) {}

        void AddToTable(CPRecord* rootRecord)
        {
           CPRecord *table = rootRecord->addChild();

         //  const CPRecordField* field = _recordType->getField(fieldName);
         //  int index = field->index();
           int num = rootRecord->getType()->getField(L"SourcePath")->index();
          if( rootRecord->getType()->getField(L"SourcePath")->index() >= 0){
              table->at(L"SourcePath") = path_;
          }
          if( rootRecord->getType()->getField(L"ModifyTime")->index()>= 0){
              table->at(L"ModifyTime") = modifyTime_;
          }
          if( rootRecord->getType()->getField(L"FileSize")->index() >= 0){
              table->at( L"FileSize") = size_;
          }
          if( rootRecord->getType()->getField(L"HashValueMD5")->index()>= 0){
              table->at( L"HashValueMD5") = md5Hash_;
          }
          if( rootRecord->getType()->getField(L"CreateTime")->index() >= 0){
              table->at( L"CreateTime") = createTime_;
          }
          if( rootRecord->getType()->getField(L"AccessTime")->index()>= 0){
               table->at(L"AccessTime") = accessTime_;
          }
          if( rootRecord->getType()->getField(L"FileName")->index() >= 0){
                 table->at(L"FileName") = name_;
          }

        }
    };
    typedef std::vector<FileItemInfo> FileItemInfos;

    class  CPFileOperate
    {
    public:
        CPFileOperate(void);
        ~CPFileOperate(void);

        //解析文件信息
        static void ParseFileInfo(PSIHANDLE itemHandle, FileItemInfo& info, bool isCalcHash = false);

        //导出文件列表时，获取单个文件的一行信息
        static std::string GetFileInfoLine(const FileItemInfo& itemInfo);

    };
}

#endif // TOOLS_FILEOPERATE_H
