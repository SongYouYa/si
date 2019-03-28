#include "stdafx.h"
#include "PHashOptionWidget2.h"
#include "siio/PHash.h"

using namespace siio;


CPHashOptionWidget2::CPHashOptionWidget2(QWidget* parent)
	: QWidget(parent)
{
	setObjectName("content_container");

	QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(15);
	std::vector<HashType> hashTypes = {
		HASH_MD5, HASH_SHA256, HASH_SHA384, HASH_SHA1, HASH_SHA224, HASH_SHA512, HASH_CRC32
	};

	for (auto hashType : hashTypes) {
		QCheckBox* checkBox = new QCheckBox(QString::fromStdWString(CPHash::getName(hashType)));
		_checkBoxs[hashType] = checkBox;
		layout->addWidget(checkBox);
		if (hashType == HASH_MD5) {
			checkBox->setChecked(true);
		}
		else if (hashType == HASH_SHA256) {
			checkBox->setChecked(true);
		}
	}

	layout->addStretch(1);
	setLayout(layout);
}

void CPHashOptionWidget2::getHashOptions(std::vector<siio::HashType>& hashTypes)
{
	for (auto it : _checkBoxs) {
		if (it.second->isChecked()) {
			hashTypes.push_back(it.first);
		}
	}
}
