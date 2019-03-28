#include "stdafx.h"
#include "PLogoWidget.h"


CPLogoWidget::CPLogoWidget(QWidget* parent)
	: QFrame(parent)
{
	setObjectName("header_logo_widget");

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(10, 0, 5, 0);

	QLabel* logoLabel = new QLabel();
	logoLabel->setPixmap(QPixmap(":/styles/icons/header/logo.png"));
	layout->addWidget(logoLabel);

#if 0
	QLabel* nameLabel = new QLabel(tr("shenkan"));
	nameLabel->setObjectName("logo_name");
	layout->addWidget(nameLabel);

	QVBoxLayout* rightLayout = new QVBoxLayout();
	rightLayout->setMargin(0);
	rightLayout->setSpacing(0);
	QLabel* productZhLabel = new QLabel(tr("SafeImager"));
	productZhLabel->setObjectName("product_name_zh");
	QLabel* productEnLabel = new QLabel("SafeImager");
	productEnLabel->setObjectName("product_name_en");
	rightLayout->addWidget(productZhLabel, 0, Qt::AlignVCenter);
	rightLayout->addWidget(productEnLabel, 0, Qt::AlignVCenter);

	layout->addLayout(rightLayout);
#endif
	layout->addStretch();

	setLayout(layout);

	setFixedWidth(200);
}
