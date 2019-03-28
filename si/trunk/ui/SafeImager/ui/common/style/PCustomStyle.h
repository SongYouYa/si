#pragma once

//¸øQToolTip¿òÌí¼ÓÔ²½Ç
class CPCustomStyle : public QProxyStyle
{
	Q_OBJECT
public:
	CPCustomStyle();

	int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const override;
};