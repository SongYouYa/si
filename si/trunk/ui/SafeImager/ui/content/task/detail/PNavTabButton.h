#pragma once

class CPNavTabButton : public QPushButton
{
	Q_OBJECT
public:
	CPNavTabButton(const QString& tabName, QWidget* parent = nullptr);

private:
	QString _tabName;
};