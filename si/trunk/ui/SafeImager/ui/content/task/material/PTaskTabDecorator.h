#pragma once


class CPTaskCountLabel : public QLabel
{
	Q_OBJECT
public:
	CPTaskCountLabel(QWidget* parent = nullptr);
};

class CPTaskTabDecorator : public QObject
{
public:
	CPTaskTabDecorator(QObject* parent);

	void decorate(QWidget* tabButton);
	void setTaskCount(int count);

private:
	CPTaskCountLabel* _countLabel;
};