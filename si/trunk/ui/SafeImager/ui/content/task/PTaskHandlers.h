#pragma once


class CPTaskHandlers
{

public:
	//给handler指定父widget，确保回调可以执行在UI线程
	//因为有些回调函数会执行界面操作，如果没有父widget，qt会有异常
	static CPTaskHandlers* get(QWidget* parent = nullptr);

	void addTask(CPTask* task);
	void delTask(CPTask* task);

private:
	CPTaskHandlers(QWidget* parent);

	QWidget* _parent;
	std::map<CPTask*, QObject*> _callbackHandlers;

	static CPTaskHandlers* _instance;
};