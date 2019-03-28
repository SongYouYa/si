#include "stdafx.h"
#include "PImageOptionWidget.h"

#include "sifile/PAffFile.h"
#include "siio/PE01File.h"

using namespace siio;
using namespace devtask;


CPImageOptionWidget::CPImageOptionWidget(QWidget* parent)
    : QWidget(parent),
      _imageType(devtask::ImageFormatOption::IMAGE_NONE)
{
	setObjectName("content_container");
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    QWidget* container = new QWidget();
    mainLayout->addWidget(container);
    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setHorizontalSpacing(5);

	_ddRadio = new QRadioButton(tr("DD format"));
    layout->addWidget(_ddRadio, 0, 0);
	_splitCheckBox = new QCheckBox(tr("split size"));
    layout->addWidget(_splitCheckBox, 0, 1);
	_splitSizeCombo = new QComboBox();
    layout->addWidget(_splitSizeCombo, 0, 2);
	_splitUnitCombo = new QComboBox();
	_splitUnitCombo->setStyleSheet("width: 30px;");
	layout->addWidget(_splitUnitCombo, 0, 3);

	_affRadio = new QRadioButton(tr("AFF format"));
    layout->addWidget(_affRadio, 1, 0);
	QLabel* affTitle = new QLabel(tr("AFF comress level"));
    layout->addWidget(affTitle, 1, 1);
	_affLevelEdit = new QLineEdit();
	layout->addWidget(_affLevelEdit, 1, 2);

	_e01Radio = new QRadioButton(tr("E01 format"));
    layout->addWidget(_e01Radio, 2, 0);
	QLabel* e01Title = new QLabel(tr("E01 compress level"));
    layout->addWidget(e01Title, 2, 1);
	_e01LevelCombo = new QComboBox();
    layout->addWidget(_e01LevelCombo, 2, 2);

	layout->setColumnMinimumWidth(0, 100);
	layout->setColumnMinimumWidth(1, 100);
    container->setLayout(layout);

    mainLayout->addStretch(1);
    setLayout(mainLayout);

	initLevelCombos();

	_splitHandler = new CPSplitWidgetHandler(this, _splitSizeCombo, _splitUnitCombo);

	_splitCheckBox->setEnabled(false);
	_splitSizeCombo->setEnabled(false);
	_splitUnitCombo->setEnabled(false);
	_affLevelEdit->setEnabled(false);
	_e01LevelCombo->setEnabled(false);

	connect(_ddRadio, &QRadioButton::toggled, this, [this](bool checked){
		if (!checked) {
			_splitCheckBox->setChecked(false);
		}
		_splitCheckBox->setEnabled(checked);
		emit signalFormatOptionChanged();
	});
	connect(_splitCheckBox, &QRadioButton::toggled, this, [this](bool checked){
		_splitSizeCombo->setEnabled(checked);
		_splitUnitCombo->setEnabled(checked);
	});
	connect(_affRadio, &QRadioButton::toggled, this, [this](bool checked){
		_affLevelEdit->setEnabled(checked);
		emit signalFormatOptionChanged();
	});
	connect(_e01Radio, &QRadioButton::toggled, this, [this](bool checked){
		_e01LevelCombo->setEnabled(checked);
		emit signalFormatOptionChanged();
	});

    _ddRadio->setChecked(true);
}

void CPImageOptionWidget::setImageType(devtask::ImageFormatOption::ImageType imageType)
{
    _imageType = imageType;
}

void CPImageOptionWidget::initLevelCombos()
{
	QRegExp numReg("[0-9]?");
	QRegExpValidator *pReg = new QRegExpValidator(numReg, this);
	_affLevelEdit->setValidator(pReg);
	_affLevelEdit->setPlaceholderText("0-9");
	_affLevelEdit->setText("1");

	_e01LevelCombo->addItem(tr("fastest"), CPE01File::COMPRESSION_FAST);
	_e01LevelCombo->addItem(tr("best"), CPE01File::COMPRESSION_BEST);
	_e01LevelCombo->addItem(tr("none"), CPE01File::COMPRESSION_NONE);
}

void CPImageOptionWidget::getOptions(devtask::ImageFormatOption& imageOptions)
{
    if (this->isEnabled()) {
        if (_ddRadio->isChecked()) {
            imageOptions.imageType = ImageFormatOption::IMAGE_DD;
        }
        else if (_affRadio->isChecked()) {
            imageOptions.imageType = ImageFormatOption::IMAGE_AFF;
			imageOptions.compressLevel = _affLevelEdit->text().toInt();
        }
        else if (_e01Radio->isChecked()) {
            imageOptions.imageType = ImageFormatOption::IMAGE_E01;
			imageOptions.compressLevel = _e01LevelCombo->currentData().toInt();
        }
        else {
            assert(false);
        }
    }
    else {
        imageOptions.imageType = _imageType;
    }

	if (_splitCheckBox->isChecked()) {
		imageOptions.splitSize = _splitHandler->GetSplitSize();
	}
}
