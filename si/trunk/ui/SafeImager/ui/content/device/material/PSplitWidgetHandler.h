#pragma once

/*/////////////////////////////////////////////////////////////////////////
内存镜像和复制界面有一些重复的功能，统一在这里实现
/////////////////////////////////////////////////////////////////////////*/


//处理镜像分片大小的界面辅助类
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

	uint64 GetSplitSize(); //获取分片大小

	protected slots:
	//编辑分片大小时,分片单位相应改变
	void slotComSizeChanged(const QString& strSize);
	//选择不同分片单位时，分片大小控件的输入限制正则需要相应的改变
	void slotComUnitChanged(const QString& strUnit);

private:
	//在字符串中，提取出表示大小的数字字符串
	QString GetSizeFromStr(const QString& wholeSizeStr);

	static QRegExpValidator splitRegValMB_; //分片单位为MB时的正则表达式
	static QRegExpValidator splitRegValGB_; //分片单位为GB时的正则表达式
	QString strSplitSize_;
	SplitUnit unit_;
	QRegExpValidator* currentRegVal_; //保存当前正则

	QComboBox* comboSize_;
	QComboBox* comboUnit_;
};
