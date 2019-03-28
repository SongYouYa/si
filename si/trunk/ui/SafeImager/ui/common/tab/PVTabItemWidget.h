#pragma once


class CPVTabItemWidget : public QFrame
{
public:
	struct Config
	{
		QString icon;
		QString text;
	};
	CPVTabItemWidget(const Config& config, QWidget* parent = nullptr);

protected:
	void resizeEvent(QResizeEvent * event) override;

	virtual void setCollapsed(bool collapsed);

	Config _config;

	QWidget* _leftSpacing;
	QLabel* _iconLabel;
	QWidget* _iconTextSpace;
	QLabel* _textLabel;

private:
	
};
