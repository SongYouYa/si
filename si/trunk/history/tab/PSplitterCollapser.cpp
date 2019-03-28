#include "stdafx.h"
#include "PSplitterCollapser.h"


CPSplitterCollapser::CPSplitterCollapser(QObject* parent)
	: QObject(parent),
	_doing(false)
{

}

void CPSplitterCollapser::setSplitter(QSplitter* splitter, const PRect& firstRect)
{
	if (splitter->count() < 1) {
		assert(false);
	}
	_splitter = splitter;
	_rect = firstRect;
}

void CPSplitterCollapser::slotToggleCollapse()
{
	if (_doing) {
		return;
	}
	_doing = true;

	QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
	animation->setDuration(300);
	animation->setEasingCurve(QEasingCurve::InOutQuint);

	//改变第一个控件的大小
	connect(animation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value){
		QRect rect = value.toRect();
		QList<int> sizes = _splitter->sizes();
		sizes[0] = rect.width();
		_splitter->setSizes(sizes);
		//std::cout << std::string("ls: ") + std::to_string(rect.width()) << std::endl;
	});
	connect(animation, &QPropertyAnimation::finished, this, [this](){
		_doing = false;
		//std::cout << std::endl;
	});

	QWidget* firstWidget = _splitter->widget(0);
	QRect g = firstWidget->geometry();
	animation->setStartValue(g);
	if (g.width() + 30 < _rect.maxWidth) {
		g.setWidth(_rect.maxWidth);
	}
	else {
		g.setWidth(_rect.minWidth);
	}
	animation->setEndValue(g);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}