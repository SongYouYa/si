#include "stdafx.h"
#include "PCustomStyle.h"

const int WindowRadius = 3;

QBitmap windowMask(const QRect& rect)
{
	QBitmap bmp(rect.width(), rect.height());
	bmp.fill();
	QPainter p(&bmp);
	p.setPen(Qt::NoPen);
	p.setBrush(Qt::black);
	p.drawRoundedRect(bmp.rect(), WindowRadius, WindowRadius);
	return bmp;
}

CPCustomStyle::CPCustomStyle()
{

}

int CPCustomStyle::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const
{
	switch (hint)
	{
	case SH_ToolTip_Mask:
    {
		QStyleHintReturnMask *mask = qstyleoption_cast<QStyleHintReturnMask *>(returnData);
		mask->region = windowMask(option->rect);
		return true;
    }
    default:
        break;
	}
	return QProxyStyle::styleHint(hint, option, widget, returnData);
}
