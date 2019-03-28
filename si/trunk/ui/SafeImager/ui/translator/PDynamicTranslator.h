#pragma once

class CPDynamicTranslator : public QObject
{
	Q_OBJECT
public:
	CPDynamicTranslator(QObject* parent = nullptr);
};

extern QString dtr(const QString& str);