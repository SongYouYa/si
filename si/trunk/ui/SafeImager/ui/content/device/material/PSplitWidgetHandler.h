#pragma once

/*/////////////////////////////////////////////////////////////////////////
�ڴ澵��͸��ƽ�����һЩ�ظ��Ĺ��ܣ�ͳһ������ʵ��
/////////////////////////////////////////////////////////////////////////*/


//�������Ƭ��С�Ľ��渨����
class CPSplitWidgetHandler : public QObject
{
	Q_OBJECT

	enum SplitUnit
	{
		Un_None,
		Un_MB,
		Un_GB
	};
public:
	CPSplitWidgetHandler(QObject* parent, QComboBox* comboSize, QComboBox* comboUnit);
	~CPSplitWidgetHandler();

	uint64 GetSplitSize(); //��ȡ��Ƭ��С

	protected slots:
	//�༭��Ƭ��Сʱ,��Ƭ��λ��Ӧ�ı�
	void slotComSizeChanged(const QString& strSize);
	//ѡ��ͬ��Ƭ��λʱ����Ƭ��С�ؼ�����������������Ҫ��Ӧ�ĸı�
	void slotComUnitChanged(const QString& strUnit);

private:
	//���ַ����У���ȡ����ʾ��С�������ַ���
	QString GetSizeFromStr(const QString& wholeSizeStr);

	static QRegExpValidator splitRegValMB_; //��Ƭ��λΪMBʱ��������ʽ
	static QRegExpValidator splitRegValGB_; //��Ƭ��λΪGBʱ��������ʽ
	QString strSplitSize_;
	SplitUnit unit_;
	QRegExpValidator* currentRegVal_; //���浱ǰ����

	QComboBox* comboSize_;
	QComboBox* comboUnit_;
};
