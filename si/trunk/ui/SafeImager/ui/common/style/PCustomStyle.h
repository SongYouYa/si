#pragma once

//��QToolTip�����Բ��
class CPCustomStyle : public QProxyStyle
{
	Q_OBJECT
public:
	CPCustomStyle();

	int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const override;
};