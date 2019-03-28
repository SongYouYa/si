#pragma once

class CPDetailButtonFactory
{
public:
	CPDetailButtonFactory();

    static QPushButton* createButton(CPTask* task);

private:

};
