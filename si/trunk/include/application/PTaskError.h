#pragma once
#include "libdevtask/PTaskError.h"


enum TASK_ERROR_DEVTASK
{
	TASK_ERROR_OPEN_INDEX = TASK_ERROR_LIBDT_END + 1, //��֤�������ļ�����
	TASK_ERROR_TOOL_LOST							//�������߶�ʧ
};

enum TASK_WARNING_DEVTASK
{
	TASK_WARNING_DEVTASK_BEGIN = TASK_WARNING_LIBDT_END + 1 //���̶�ʧ
};