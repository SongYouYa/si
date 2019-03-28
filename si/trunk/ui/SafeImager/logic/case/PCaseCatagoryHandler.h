#pragma once


//������������
//�����������仯�ǣ���Щ��ѡ����Ҫ�ı�
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
