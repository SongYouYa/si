#pragma once


class CPTaskHandlers
{

public:
	//��handlerָ����widget��ȷ���ص�����ִ����UI�߳�
	//��Ϊ��Щ�ص�������ִ�н�����������û�и�widget��qt�����쳣
	static CPTaskHandlers* get(QWidget* parent = nullptr);

	void addTask(CPTask* task);
	void delTask(CPTask* task);

private:
	CPTaskHandlers(QWidget* parent);

	QWidget* _parent;
	std::map<CPTask*, QObject*> _callbackHandlers;

	static CPTaskHandlers* _instance;
};