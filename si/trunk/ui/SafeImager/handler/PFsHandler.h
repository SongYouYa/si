#pragma once


//�ļ�ϵͳ���صĴ���
class CPFsHandler : public QObject
{
	Q_OBJECT
public:
	static CPFsHandler* get();

	void loadFs();

private:
	CPFsHandler();

	bool _fsLoaded;

	static CPFsHandler* _instance;
};