#pragma once


class CPCaseHandler : public QObject
{
	Q_OBJECT
public:
	CPCaseHandler();
	//�ж�case�Ƿ񴴽�
	static bool checkCaseCreated();

    static bool openCase(QWidget* parent);
	
};
