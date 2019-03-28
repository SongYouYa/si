#pragma once


//文件系统加载的处理
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