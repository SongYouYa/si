#pragma once


class CPStatusBar : public QFrame
{
	Q_OBJECT
public:
	CPStatusBar(QWidget* parent = nullptr);

private:
	void updateCaseInfo();

	QLabel* _caseNameValue;
	QLabel* _suspectValue;
};
