class CPTabHeaderItemWidget : public CPTabItemWidget
{
	Q_OBJECT
public:
	CPTabHeaderItemWidget(const Config& config, QWidget* parent = nullptr);

signals:
	void signalToggleCollapse();

protected:
	void setCollapsed(bool collapsed) override;

	QToolButton* _collapseButton;
};