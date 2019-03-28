#include "stdafx.h"
#include "ui/common/progress/PLineLoadingWidget.h"


namespace puic
{
	CPLineLoadingWidget::CPLineLoadingWidget(QWidget* parent)
		: CPLoadingWidgetBase(parent),
		_parent(parent)
	{
		setObjectName("line_loading_widget");
		setFixedSize(QSize(257, 3));

		_animation = new QPropertyAnimation(this, "pos");
		_animation->setLoopCount(-1);

		_parent->installEventFilter(this);
		setVisible(false);
	}

	void CPLineLoadingWidget::setLoading(bool loading)
	{
		if (loading) {
			this->show();
			updateAnimate();
			_animation->start();
		}
		else {
			this->hide();
			_animation->stop();
		}
	}

	bool CPLineLoadingWidget::eventFilter(QObject * watched, QEvent * event)
	{
		QEvent::Type et = event->type();
		if (et == QEvent::Resize) {
			updateAnimate();
		}
		return QFrame::eventFilter(watched, event);
	}

	void CPLineLoadingWidget::updateAnimate()
	{
		QSize parentSize = _parent->size();
		QSize tSize = this->size();
		int h = parentSize.height() - this->size().height() - 1;
		QPoint startPos(-tSize.width(), h);
		QPoint endPos(parentSize.width(), h);

		_animation->setDuration(parentSize.width() * 2);
		_animation->setStartValue(startPos);
		_animation->setEndValue(endPos);
	}
}