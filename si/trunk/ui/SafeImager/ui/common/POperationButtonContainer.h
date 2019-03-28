#pragma once


class CPOperationButtonContainer : public QFrame
{
public:
	CPOperationButtonContainer(QWidget* parent = nullptr);

	void addLeftWidget(QWidget* widget);
	void addRightWidget(QWidget* widget);

private:
	QHBoxLayout* _leftLayout;
	QHBoxLayout* _rightLayout;
};