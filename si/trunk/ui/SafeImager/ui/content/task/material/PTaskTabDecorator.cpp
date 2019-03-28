#include "stdafx.h"
#include "PTaskTabDecorator.h"


CPTaskCountLabel::CPTaskCountLabel(QWidget* parent)
	: QLabel(parent)
{
	setObjectName("task_tab_count");
	const QSize WidgetSize = QSize(16, 16);
	setFixedSize(WidgetSize);

	this->setAlignment(Qt::AlignCenter);
}

CPTaskTabDecorator::CPTaskTabDecorator(QObject* parent)
	: QObject(parent),
	_countLabel(nullptr)
{

}

void CPTaskTabDecorator::decorate(QWidget* tabButton)
{
	_countLabel = new CPTaskCountLabel(tabButton);
	QSize tabSize = tabButton->size();
	QPoint pos;
	pos.setX(tabSize.width() * 3 / 5);
	pos.setY(tabSize.height() / 8);
	_countLabel->move(pos);
}

void CPTaskTabDecorator::setTaskCount(int count)
{
	if (_countLabel) {
		if (count == 0) {
			_countLabel->setVisible(false);
		}
		else {
			_countLabel->setVisible(true);
			_countLabel->setText(QString::number(count));
		}
	}
}