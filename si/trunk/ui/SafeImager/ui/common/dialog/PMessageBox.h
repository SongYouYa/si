#pragma once
#include "./PDialog2.h"


class CPMessageBox : public CPDialog2
{
	Q_OBJECT
		CPMessageBox(QWidget* parent = nullptr);
public:
	static int info(const QString& msg, QWidget* parent = nullptr);
	static int warning(const QString& msg, QWidget* parent = nullptr);
	static int question(const QString& msg, QWidget* parent = nullptr);
	static int error(const QString& msg, QWidget* parent = nullptr);
	static int questionEx(QWidget* parent, const QString& msg,
		const QString& okText, const QString& thirdText);

private:
	void setMsg(const QString& msg);
	QLabel* _iconLabel;
	QLabel* _msgLabel;
};