#pragma once
#include "./PToolsHandler.h"

class CPToolContainer : public QFrame
{
	Q_OBJECT
public:
	CPToolContainer(QWidget* parent = nullptr);
	void setMainWindow(QWidget* widget);

private:

	CPToolsHandler* _toolsHandler;
};