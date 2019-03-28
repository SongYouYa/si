CPTabHeaderItemWidget::CPTabHeaderItemWidget(const Config& config, QWidget* parent)
	: CPTabItemWidget(config, parent)
{
	layout()->setContentsMargins(5, 18, 10, 18);

	setObjectName("nav_logo_widget");
	_leftSpacing->setFixedWidth(10);
	_iconLabel->setObjectName("logo_icon");
	_iconLabel->setFixedSize(QSize(25, 25));
	_textLabel->setObjectName("logo_text");

#ifdef SI_VERSION_MAC
    _textLabel->setStyleSheet("font-size: 16px;");
#endif

	_collapseButton = new QToolButton();
	_collapseButton->setObjectName("collapse");
	_collapseButton->setFixedSize(QSize(16, 17));
	layout()->addWidget(_collapseButton);

	connect(_collapseButton, &QPushButton::clicked, this, [this](){
		emit signalToggleCollapse();
	});
}

void CPTabHeaderItemWidget::setCollapsed(bool collapsed)
{
	CPTabItemWidget::setCollapsed(collapsed);
	_iconLabel->setVisible(!collapsed);
}
