#include "stdafx.h"
#include "PDetailButtonFactory.h"
#include "POpenPathButton.h"
#include "PNavTabButton.h"

CPDetailButtonFactory::CPDetailButtonFactory()
{

}

QPushButton* CPDetailButtonFactory::createButton(CPTask* task){
	
	if (L"task.deviceimage" == task->type()) {
		devtask::DeviceImageParam param = boost::any_cast<devtask::DeviceImageParam>(*task->param());
		return new CPOpenPathButton(param.imagePath);
	}
	else if (L"task.memoryimage" == task->type()) {
		devtask::MemoryImageParam param = boost::any_cast<devtask::MemoryImageParam>(*task->param());
		return new CPOpenPathButton(param.imagePath);
	}
	else if (L"task.fileexport" == task->type()) {
		devtask::FileExportParam param = boost::any_cast<devtask::FileExportParam>(*task->param());
		return new CPOpenPathButton(param.exportDir);
	}
	else if (L"task.apps" == task->type()) {
		return new CPNavTabButton(PTAB_APP);
	}
	else {
		return NULL;
	}
}
