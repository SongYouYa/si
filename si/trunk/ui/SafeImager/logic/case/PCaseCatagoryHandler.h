#pragma once


//案件大类别管理
//当案件大类别变化是，有些勾选项需要改变
class CPCaseCatagoryHandler : public QObject
{
	Q_OBJECT
public:
	static CPCaseCatagoryHandler* get();
	bool loadIni(const std::wstring& filePath);

	void setCatagory(const QString& catagory);
	void getCatagoryApps(std::set<std::wstring>& apps);

signals:
	void signalCatagoryChanged();

private:
	CPCaseCatagoryHandler();

	QString _iniPath;
	QString _catagory;
	std::set<std::wstring> _defaultApps;

	static CPCaseCatagoryHandler* _instance;
};
