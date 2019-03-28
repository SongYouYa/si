#pragma once

class CPDropDownWidget : public QPushButton
{
	Q_OBJECT
public:
	CPDropDownWidget(QWidget* parent = nullptr);

private slots:
	void slotAbout();
	void slotOpenHelp();
	void slotUrl();

private:
	void initMenu();
	QMenu* _menu;
};