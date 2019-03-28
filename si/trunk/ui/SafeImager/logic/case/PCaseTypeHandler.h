#pragma once


//案件类型处理
class CPCaseTypeHandler
{
public:
	static CPCaseTypeHandler* get();
	bool loadTxt(const std::wstring& filePath);

	//包含所有案件类型的详细信息
	QStringListModel* getDetailModel() { return _detailModel; }
	//只包含大类别的信息
	QStringListModel* getCataModel() { return _catagoryModel; }
	//根据选择的案件类型字符串，获取案件大类
	QString getCatagory(const QString& numberText);

private:
	CPCaseTypeHandler();

	QStringListModel* _detailModel; //包含所有案件类型的详细信息
	QStringListModel* _catagoryModel; //只包含大类别的信息
	std::map<QString, QString> _parentMap; //包含父子关系的map

	static CPCaseTypeHandler* _instance;
};
