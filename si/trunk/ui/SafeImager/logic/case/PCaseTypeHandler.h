#pragma once


//�������ʹ���
class CPCaseTypeHandler
{
public:
	static CPCaseTypeHandler* get();
	bool loadTxt(const std::wstring& filePath);

	//�������а������͵���ϸ��Ϣ
	QStringListModel* getDetailModel() { return _detailModel; }
	//ֻ������������Ϣ
	QStringListModel* getCataModel() { return _catagoryModel; }
	//����ѡ��İ��������ַ�������ȡ��������
	QString getCatagory(const QString& numberText);

private:
	CPCaseTypeHandler();

	QStringListModel* _detailModel; //�������а������͵���ϸ��Ϣ
	QStringListModel* _catagoryModel; //ֻ������������Ϣ
	std::map<QString, QString> _parentMap; //�������ӹ�ϵ��map

	static CPCaseTypeHandler* _instance;
};
