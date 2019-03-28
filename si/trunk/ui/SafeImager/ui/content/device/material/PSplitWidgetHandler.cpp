#include "stdafx.h"
#include "PSplitWidgetHandler.h"


#define GBBYTE (1024*1024*1024)
#define MBBYTE (1024*1024)

#define SPLIT_GB_REGEX "([0-9]{0,10}[\\.]?[0-9]?)[' ']*(G.*)?"
#define SPLIT_MB_REGEX "([0-9]{0,13})[' ']*(M.*)?"


QRegExpValidator CPSplitWidgetHandler::splitRegValMB_(QRegExp(SPLIT_MB_REGEX)); //��λΪGBʱ��������һλС��
QRegExpValidator CPSplitWidgetHandler::splitRegValGB_(QRegExp(SPLIT_GB_REGEX)); //��λΪMBʱ��ֻ��Ϊ����

CPSplitWidgetHandler::CPSplitWidgetHandler(QObject* parent, QComboBox* comboSize, QComboBox* comboUnit)
	: QObject(parent), unit_(Un_MB), comboSize_(comboSize), comboUnit_(comboUnit)
{
	comboSize_->setEditable(true);

	comboSize_->addItem("700 MB (CD)");
	comboSize_->addItem("4095 MB (FAT32)");
	comboSize_->addItem("4.7 GB (DVD)");
	comboSize_->addItem("8.6 GB (DVD)");

	comboUnit_->addItem("MB");
	comboUnit_->addItem("GB");

	//set text color
	QPalette palette;
	palette.setColor(QPalette::Text, QColor(0x5A, 0x5B, 0x5B));
	comboSize_->setPalette(palette);

	comboSize_->setCompleter(NULL); //��ֹ�Զ���ȫ
	connect(comboSize_, SIGNAL(editTextChanged(const QString&)), this, SLOT(slotComSizeChanged(const QString&)));
	connect(comboUnit_, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(slotComUnitChanged(const QString&)));

	comboSize_->setCurrentIndex(0);
	comboSize_->setValidator(&splitRegValMB_); //����������������
	strSplitSize_ = comboSize_->currentText(); //��ʼ����Ƭ��С�ַ���
	currentRegVal_ = &splitRegValMB_;

	comboUnit_->hide();
}

CPSplitWidgetHandler::~CPSplitWidgetHandler()
{

}

void CPSplitWidgetHandler::slotComSizeChanged(const QString& strSize)
{
	if (strSize.contains("GB", Qt::CaseSensitive))
	{
		comboUnit_->setCurrentIndex(1);
		comboUnit_->hide();
	}
	else if (strSize.contains("MB", Qt::CaseSensitive))
	{
		comboUnit_->setCurrentIndex(0);
		comboUnit_->hide();
	}
	else
	{
		comboUnit_->show(); //��ǰ��û�е�λʱ����ʾ��λ�ؼ�
	}

	strSplitSize_ = strSize;
}

void CPSplitWidgetHandler::slotComUnitChanged(const QString& strUnit)
{
	QString curstr = comboSize_->currentText();
	QString strSize = GetSizeFromStr(curstr);
	if (strUnit.contains("GB", Qt::CaseSensitive))
	{
		currentRegVal_ = &splitRegValGB_;
		unit_ = Un_GB;
		comboSize_->setValidator(&splitRegValGB_);
		if (curstr.contains("GB", Qt::CaseSensitive)
			|| curstr.contains("MB", Qt::CaseSensitive))
		{
			return;
		}

		curstr = strSize;
	}
	else if (strUnit.contains("MB", Qt::CaseSensitive))
	{
		currentRegVal_ = &splitRegValMB_;
		unit_ = Un_MB;
		comboSize_->setValidator(&splitRegValMB_);
		if (curstr.contains("GB", Qt::CaseSensitive)
			|| curstr.contains("MB", Qt::CaseSensitive))
		{
			return;
		}

		//�Զ����Сʱ�����ı䵥λ������ַ������д���һ��
		double dsize = strSize.toDouble();
		uint64 isize = dsize;
		curstr = QString::number(isize);
	}
	comboSize_->setEditText(curstr + " "); //�ı�һ�����ݣ�ʹValidator��Ч
	comboSize_->setEditText(curstr);
}

uint64 CPSplitWidgetHandler::GetSplitSize()
{
	QRegExp reg = currentRegVal_->regExp();
	if (!reg.exactMatch(strSplitSize_))
	{
		return 0;
	}

	QString sizestr = reg.cap(1);
	bool bt = false;
	if (unit_ == Un_MB)
	{
		bool bt = false;
		uint64 size = sizestr.toLongLong(&bt);
		if (!bt)
		{
			return 0;
		}
		size = size * MBBYTE;
		return size;
	}
	else if (unit_ == Un_GB)
	{
		bool bt = false;
		double size = sizestr.toDouble(&bt);
		if (bt)
		{
			uint64 mbsize = size * 1024; //��Ƭ��СҪ��Ϊ����MB
			return mbsize*MBBYTE;
		}
	}
	return 0;
}

QString CPSplitWidgetHandler::GetSizeFromStr(const QString& wholeSizeStr)
{
	QString strSize;
	QRegExp reg = splitRegValGB_.regExp();
	if (reg.exactMatch(wholeSizeStr))
	{
		strSize = reg.cap(1);
		return strSize;
	}
	reg = splitRegValMB_.regExp();
	if (reg.exactMatch(wholeSizeStr))
	{
		strSize = reg.cap(1);
		return strSize;
	}
	return strSize;
}
