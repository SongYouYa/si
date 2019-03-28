#include "stdafx.h"
#include "application/MacBasic/PMacBasicTask.h"
#include "SystemInfo/include/systeminfo/GetComputerName.h"
#include "SystemInfo/include/systeminfo/SysBaseInfoAnalyzer.h"
#include "SystemInfo/include/systeminfo/DeviceInfoAnalyzer.h"
#include "SystemInfo/include/systeminfo/PartitionAnalyzer.h"
#include "SystemInfo/include/systeminfo/ShareListAnalyzer.h"
#include "SystemInfo/include/systeminfo/UseInfoAnalyzer.h"
#include "MacIMAnalyzer/include/macim/skype/SkypeHelper.h"
#include "MacIMAnalyzer/include/macim/adium/AdiumTaskHelper.h"
#include "MacIMAnalyzer/include/macim/imessage/IMessageTaskHelper.h"
#include "SIBrowser/include/SIBrowser/SafariParser.h"
////#include"MacMailAnalyzer/include/macmail/MailTaskHelper.h"
#include "SIBrowser/include/SIBrowser/ChromeParser.h"


namespace macbasic
{
CPMacBasicTask::CPMacBasicTask()
{
}
void CPMacBasicTask::doTask(CPTask* task)
{

    std::cout <<"work the task!"<<std::endl;
    CPTaskStatusCtrl* statusCtrl = task->statusCtrl();
    CPRecord* rootRecord = task->rootRecord();
    bool ret = GetComputerName::analyze(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }
    ret = DeviceInfoAnalyzer::analyze(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }

    ret = CSysBaseInfoAnalyzer::analyze(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }

    ret = PartitionAnalyzer::analyze(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }
    ShareListAnalyzer shareListAnalyze;
    ret = shareListAnalyze.analyze(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }

    UseInfoAnalyzer userInfo;
    ret = userInfo.GetUseInfo(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }

    ret = SkypeHelper::AnalyzerSkypeAccount(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }
    ret = SkypeHelper::AnalyzerSkypeContact(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }
    ret = SkypeHelper::AnalyzerSkypeMsgs(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }

    CSafariParser csafaiParser;
    ret =  csafaiParser.GetBookmark(rootRecord);
    if(!ret){
        statusCtrl->fail();
    }

    ret =  csafaiParser.GetDownload(rootRecord);
    if(!ret){
        statusCtrl->fail();
    }

    ret =  csafaiParser.GetCache(rootRecord);
    if(!ret){
        statusCtrl->fail();
    }
    ret =  csafaiParser.GetCookie(rootRecord);
    if(!ret){
        statusCtrl->fail();
    }
    ret =  csafaiParser.GetHistory(rootRecord);
    if(!ret){
        statusCtrl->fail();
    }
    CChromeParser chromeParser;
    ret = chromeParser.GetBookmark(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }
    ret = chromeParser.GetCache(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }
    ret = chromeParser.GetCookie(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }
    ret = chromeParser.GetDownload(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }
    ret = chromeParser.GetForm(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }

    ret = AdiumTaskHelper::GetAccountInfo(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }
    ret = AdiumTaskHelper::GetContactInfo(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }
    ret = AdiumTaskHelper::GetMsgInfo(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }

    ret = IMessageTaskHelper::GetIMessageAccountInfo(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }
    ret = IMessageTaskHelper::GetIMessageAccountRecords(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }
    ret = IMessageTaskHelper::GetIMessageMsgInfo(rootRecord);
    if (!ret) {
        statusCtrl->fail();
    }

    //    MailTaskHelper mailTaskHelper;
    //    ret =mailTaskHelper.GetMailAccount(rootRecord);
    //    if (!ret) {
    //        statusCtrl->fail();
    //    }
    //    ret =mailTaskHelper.GetMailContacts(rootRecord);
    //    if (!ret) {
    //        statusCtrl->fail();
    //    }
    //    ret =mailTaskHelper.GetMailRecords(rootRecord);
    //    if (!ret) {
    //        statusCtrl->fail();
    //    }
}
}
