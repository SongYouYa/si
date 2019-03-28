#include "stdafx.h"
#include "PSearchEdit.h"


const QSize IconSize = QSize(15, 15);

CPSearchEdit::CPSearchEdit(QWidget* parent)
	: QWidget(parent)
{
	setObjectName("search_edit");
	setFixedWidth(250);

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);

	_lineEdit = new QLineEdit();
	layout->addWidget(_lineEdit, 1);

	_searchLabel = new QLabel(_lineEdit);
	_searchLabel->setObjectName("search_label");
	_searchLabel->setFixedSize(IconSize);

	_cancelButton = new QPushButton(_lineEdit);
	_cancelButton->setObjectName("cancel_button");
	_cancelButton->setFixedSize(IconSize);
	_cancelButton->setVisible(false);
	_cancelButton->setCursor(Qt::PointingHandCursor);

	setLayout(layout);

	_timer = new QTimer(this);
	_timer->setInterval(500);
	_timer->setSingleShot(true);

	connect(_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(slotTextChanged(const QString&)));
	connect(_cancelButton, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
	connect(_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
}

void CPSearchEdit::showEvent(QShowEvent* e)
{
	QSize editSize = _lineEdit->size();
	QSize searchSize = _searchLabel->size();
	QPoint pos;
	pos.setX(editSize.width() - searchSize.width() - 6);
	pos.setY(4);
	_searchLabel->move(pos);
	_cancelButton->move(pos);
	QWidget::showEvent(e);
}

void CPSearchEdit::keyPressEvent(QKeyEvent * event)
{
	int key = event->key();
	if (key == Qt::Key_Enter || key == Qt::Key_Return) {
		emit signalEnterPressed();
	}
	QWidget::keyPressEvent(event);
}

void CPSearchEdit::slotTimeout()
{
	emit signalTextChanged(_lineEdit->text());
}

void CPSearchEdit::slotTextChanged(const QString& text)
{
	if (text.isEmpty()) {
		_searchLabel->show();
		_cancelButton->hide();
	}
	else {
		_searchLabel->hide();
		_cancelButton->show();
	}
	_timer->start();
}

void CPSearchEdit::slotCancelClicked()
{
	_lineEdit->clear();
	emit signalCancelClicked();
}
