#pragma once
#include "./PSplitWidgetHandler.h"

class CPImageOptionWidget : public QWidget
{
	Q_OBJECT
public:
	CPImageOptionWidget(QWidget* parent = nullptr);

    //zip format need set
    void setImageType(devtask::ImageFormatOption::ImageType imageType);

	void getOptions(devtask::ImageFormatOption& imageOptions);

signals:
	void signalFormatOptionChanged();

private:
	void initLevelCombos();

	QRadioButton* _ddRadio;
	QCheckBox* _splitCheckBox;
	QComboBox* _splitSizeCombo;
	QComboBox* _splitUnitCombo;
	QRadioButton* _affRadio;
	QLineEdit* _affLevelEdit;
	QRadioButton* _e01Radio;
	QComboBox* _e01LevelCombo;

	CPSplitWidgetHandler* _splitHandler;

    devtask::ImageFormatOption::ImageType _imageType;
};
