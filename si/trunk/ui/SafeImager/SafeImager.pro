include(../../lib.pri)

TEMPLATE = app
QT += core gui network widgets xml

mac {
QMAKE_MAC_SDK
TARGET = SafeImager
QMAKE_INFO_PLIST = MyInfo.plist
CONFIG(debug,debug|release):{
    DESTDIR = $$_PRO_FILE_PWD_/../../libs-build/debug/
}
CONFIG(release,debug|release):{
    DESTDIR = $$_PRO_FILE_PWD_/../../libs-build/release/
}
}
else {
TARGET = eForen
}

RESOURCES += $$_PRO_FILE_PWD_/main.qrc

#由于qtpro连接库顺序为：被依赖的库在后面，所以依赖app库的需要放在前面
LIBS += -lassistant \
    -lAnalyzerAssistant

LIBS += -lWindowsAnalyzer \
        -lsiregistry \
        -lMacAnalyzer

LIBS += \
    -lDeviceTask \
    -lCommonAnalyzer \
    -lreport \
    -lplog \
    -ltask \
    -lsidevice \
    -lDeviceEnumrator \
    -lfilesystemshell \
    -lTemplateFile \
    -lFileAnalyzer \
    -lsiutility \
    -lutility \
    -lsiio \
    -luicommon \
    -lqtutility \
    -lcontroller \
    -llibdevtask \
    -lbase \
    -lsifile \
    -lexpression \
    -lsilicchecker \
    -lLicenseChecker \
    -lsicommon \
    -lScreenRecorderLoader \
    -lsinetwork

#thrid_party libs
LIBS  += \
    -lboost_date_time-mt \
    -lboost_filesystem-mt \
    -lboost_program_options-mt \
    -lboost_regex-mt \
    -lboost_system-mt \
    -lboost_thread-mt \
    -lboost_iostreams-mt \
    -llog4cpp \
    -lewf \
    -lpugixml \
    -lQDeviceWatcher \
    -lplistcpp \
    -lsqlite3 \
    -lafflib \
    -ljsoncpp \
    -lctemplate

mac {
LIBS += -lz -lbz2 -liconv \
    -framework IOKit -framework CoreFoundation \
    -lcurl -lexpat \#afflib
    -larchive
} else {
LIBS += -lLicAuthClient -lLicAuthCommon \
    -ldl
}

LIBS += -lcrypto -lssl

HEADERS += \
    PMainWindow.h \
    PTabNameDefs.h \
    resource.h \
    stdafx.h \
    logic/PAppContext.h \
    ui/content/case/PCaseInfoDialog.h \
    ui/content/case/PCaseInfoParser.h \
    ui/content/device/handler/PDeviceViewHandler.h \
    ui/content/device/material/PImageOptionWidget.h \
    ui/content/device/material/PSplitWidgetHandler.h \
    ui/content/device/PDeviceCloneWidget.h \
    ui/content/device/PDeviceCopyView.h \
    ui/content/device/PDeviceImageWidget.h \
    ui/content/filesytem/PFilesystemView.h \
    ui/content/task/PTaskListWidget.h \
    ui/content/task/PTaskView.h \
    ui/translator/PDynamicTranslator.h \
    ../../include/uicommon/filesystem/PFsTreelistWidget.h \
    ../../include/uicommon/filter/PFilterBarWidget.h \
    ../../include/uicommon/record/PAppTreelistWidget.h \
    ../../include/uicommon/filesystem/PFsTreeWidget.h \
    handler/PTaskCacheHandler.h \
    ui/common/POperationButton.h \
    ui/common/POperationButtonContainer.h \
    ui/content/device/PDeviceTabNameDefs.h \
    ui/content/device/PImageWidgetBase.h \
    ui/content/device/material/PHashOptionWidget2.h \
    ui/content/apps/PAppView.h \
    ui/common/tab/PLevel2TabBar.h \
    logic/PDeviceLoader.h \
    logic/PReportStatus.h \
    ui/content/report/PReportCreater.h \
    handler/PFsHandler.h \
    handler/PCaseHandler.h \
    ui/content/filesytem/export/PExportOptionDialog.h \
    ui/content/filesytem/export/PExportFileSizeCalculator.h \
    handler/PDeviceTaskHandler.h \
    ui/common/PFilePathChecker.h \
    ui/content/filesytem/filter/PFsFilterBar.h \
    ui/net/PNetShareDialog.h \
    ui/content/task/detail/PDetailButtonFactory.h \
    ui/content/task/detail/PNavTabButton.h \
    ui/content/task/detail/POpenPathButton.h \
    handler/PContextHandler.h \
    ui/net/PNetLicenseDialog.h \
    ui/net/PNetLicHandler.h \
    ui/net/PIpDialog.h \
    ui/content/device/material/PVolumePasswordDialog.h \
    ui/header/PAboutDialog.h \
    ui/header/PDropDownWidget.h \
    ui/header/PHeaderWidget.h \
    ui/header/PLogoWidget.h \
    ui/header/PPageTabButton.h \
    ui/header/PTabBarWidget.h \
    ui/header/PToolContainer.h \
    ui/header/PToolsHandler.h \
    ui/common/widget/PDirSelectWidget.h \
    ui/common/widget/PHSplitLine.h \
    ui/common/widget/PWidgetTitleWrapper.h \
    ui/common/tab/PVTabBar.h \
    ui/common/tab/PVTabItemWidget.h \
    ui/content/device/PMemoryImageDialog.h \
    ui/content/device/PMemoryImageWidget.h \
    ui/content/device/PMemoryProgressDialog.h \
    ui/content/task/material/PTaskTabDecorator.h \
    ui/content/task/PDoneTaskItemWidget.h \
    ui/content/task/PDoneTaskListWidget.h \
    ui/content/task/PRunningTaskItemWidget.h \
    ui/content/task/PRunningTaskListWidget.h \
    ui/content/task/PTaskHandlers.h \
    ui/content/task/PTaskItemWidgetBase.h \
    ui/common/style/PCustomStyle.h \
    ui/common/dialog/PDialog2.h \
    ui/common/dialog/PMessageBox.h \
    ui/common/edit/PSearchEdit.h \
    ui/common/filter/PFilterBarWidget.h \
    ui/common/filter/dialog/PExpWidget.h \
    ui/common/filter/dialog/PFieldNameModel.h \
    ui/common/filter/dialog/PFilterDef.h \
    ui/common/filter/dialog/PFilterExpDialog.h \
    ui/common/filter/dialog/POperatorModel.h \
    ui/common/filter/dialog/PValue.h \
    ui/status/PStatusBar.h \
    ui/content/report/PExportReportDialog.h \
    ui/content/report/PReportCreater2.h \
    ui/content/report/PReportCreaterBase.h \
    ui/content/report/PReportRecordHandler.h \
    ui/content/report/PReportView.h \
    ui/common/progress/PLineLoadingWidget.h \
    logic/case/PCaseCatagoryHandler.h \
    logic/case/PCaseTypeHandler.h


SOURCES += \
    main.cpp \
    PMainWindow.cpp \
    stdafx.cpp \
    logic/PAppContext.cpp \
    ui/content/case/PCaseInfoDialog.cpp \
    ui/content/case/PCaseInfoParser.cpp \
    ui/content/device/handler/PDeviceViewHandler.cpp \
    ui/content/device/material/PImageOptionWidget.cpp \
    ui/content/device/material/PSplitWidgetHandler.cpp \
    ui/content/device/PDeviceCloneWidget.cpp \
    ui/content/device/PDeviceCopyView.cpp \
    ui/content/device/PDeviceImageWidget.cpp \
    ui/content/filesytem/PFilesystemView.cpp \
    ui/content/task/PTaskListWidget.cpp \
    ui/content/task/PTaskView.cpp \
    ui/translator/PDynamicTranslator.cpp \
    handler/PTaskCacheHandler.cpp \
    ui/common/POperationButton.cpp \
    ui/common/POperationButtonContainer.cpp \
    ui/content/device/PImageWidgetBase.cpp \
    ui/content/device/material/PHashOptionWidget2.cpp \
    ui/content/apps/PAppView.cpp \
    ui/common/PFilePathChecker.cpp \
    ui/common/tab/PLevel2TabBar.cpp \
    logic/PDeviceLoader.cpp \
    logic/PReportStatus.cpp \
    ui/content/report/PReportCreater.cpp \
    handler/PFsHandler.cpp \
    handler/PCaseHandler.cpp \
    ui/content/filesytem/export/PExportOptionDialog.cpp \
    ui/content/filesytem/export/PExportFileSizeCalculator.cpp \
    handler/PDeviceTaskHandler.cpp \
    ui/content/filesytem/filter/PFsFilterBar.cpp \
    ui/net/PNetShareDialog.cpp \
    ui/content/task/detail/PDetailButtonFactory.cpp \
    ui/content/task/detail/PNavTabButton.cpp \
    ui/content/task/detail/POpenPathButton.cpp \
    handler/PContextHandler.cpp \
    ui/net/PNetLicenseDialog.cpp \
    ui/net/PNetLicHandler.cpp \
    ui/net/PIpDialog.cpp \
    ui/content/device/material/PVolumePasswordDialog.cpp \
    ui/header/PAboutDialog.cpp \
    ui/header/PDropDownWidget.cpp \
    ui/header/PHeaderWidget.cpp \
    ui/header/PLogoWidget.cpp \
    ui/header/PPageTabButton.cpp \
    ui/header/PTabBarWidget.cpp \
    ui/header/PToolContainer.cpp \
    ui/header/PToolsHandler.cpp \
    ui/common/widget/PDirSelectWidget.cpp \
    ui/common/widget/PHSplitLine.cpp \
    ui/common/widget/PWidgetTitleWrapper.cpp \
    ui/common/tab/PVTabBar.cpp \
    ui/common/tab/PVTabItemWidget.cpp \
    ui/content/device/PMemoryImageDialog.cpp \
    ui/content/device/PMemoryImageWidget.cpp \
    ui/content/device/PMemoryProgressDialog.cpp \
    ui/content/task/material/PTaskTabDecorator.cpp \
    ui/content/task/PDoneTaskItemWidget.cpp \
    ui/content/task/PDoneTaskListWidget.cpp \
    ui/content/task/PRunningTaskItemWidget.cpp \
    ui/content/task/PRunningTaskListWidget.cpp \
    ui/content/task/PTaskHandlers.cpp \
    ui/content/task/PTaskItemWidgetBase.cpp \
    ui/common/style/PCustomStyle.cpp \
    ui/common/dialog/PDialog2.cpp \
    ui/common/dialog/PMessageBox.cpp \
    ui/common/edit/PSearchEdit.cpp \
    ui/common/filter/PFilterBarWidget.cpp \
    ui/common/filter/dialog/PExpWidget.cpp \
    ui/common/filter/dialog/PFieldNameModel.cpp \
    ui/common/filter/dialog/PFilterExpDialog.cpp \
    ui/common/filter/dialog/POperatorModel.cpp \
    ui/status/PStatusBar.cpp \
    ui/content/report/PExportReportDialog.cpp \
    ui/content/report/PReportCreater2.cpp \
    ui/content/report/PReportCreaterBase.cpp \
    ui/content/report/PReportRecordHandler.cpp \
    ui/content/report/PReportView.cpp \
    ui/common/progress/PLineLoadingWidget.cpp \
    logic/case/PCaseCatagoryHandler.cpp \
    logic/case/PCaseTypeHandler.cpp
