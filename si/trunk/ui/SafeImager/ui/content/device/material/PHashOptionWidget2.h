#pragma once
#include "siio/PEnumDef.h"


class CPHashOptionWidget2 : public QWidget
{
public:
	CPHashOptionWidget2(QWidget* parent = nullptr);
	void getHashOptions(std::vector<siio::HashType>& hashTypes);

private:
	std::map<siio::HashType, QCheckBox*> _checkBoxs;
};
