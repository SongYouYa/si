#include "stdafx.h"
#include "sicommon/PUtils.h"
#include "siutility/PDirHelper.h"
#include "application/DeviceTask/PScreenRecordTask.h"
#include "ScreenRecorderLoader/PScreenRecorderLoader.h"

using namespace pdevice;

namespace devtask
{
	CPScreenRecordTask::CPScreenRecordTask(){

	}

	void CPScreenRecordTask::doTask(CPTask* task)
	{
		CPRecord* record = task->rootRecord();
		record->at(L"record_start_time") = CPTime::GetCurrentTime();
		TaskParamPtr taskParamPtr = task->param();
		VideoParam param = boost::any_cast<VideoParam>(*taskParamPtr);
		CPTaskStatusCtrl* statusCtrl = task->statusCtrl();
		video::CPScreenRecorderLoader screenRecord;
		screenRecord.setScreenRecorderPath(param.videoExePath);
		screenRecord.addErrorCallback([statusCtrl](){
			statusCtrl->fail();
		});
		bool isSucess = screenRecord.start(param.videoDirectorPath);
		if (!isSucess){
			statusCtrl->fail();
		}
		while (!statusCtrl->isCanceled() &&
			!statusCtrl->isFailed()){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		screenRecord.stop();
		record->at(L"record_endtime") = CPTime::GetCurrentTime();
	}
}
