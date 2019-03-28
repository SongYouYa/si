#pragma once


class CPCaseHandler : public QObject
{
	Q_OBJECT
public:
	CPCaseHandler();
	//≈–∂œcase «∑Ò¥¥Ω®
	static bool checkCaseCreated();

    static bool openCase(QWidget* parent);
	
};
