#pragma once

class CPTaskCacheHandler : public QObject
{
	Q_OBJECT
public:
	CPTaskCacheHandler();

	static void handleCachedTasks(bool prompt);

private:
	static CPTask* createTask(const std::wstring& taskType,
		const std::wstring& taskId,
		TaskParamPtr param);
};

