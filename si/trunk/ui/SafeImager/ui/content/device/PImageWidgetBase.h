#pragma once
#include "uicommon/device/PDeviceUsbDecorator.h"
#include "ui/common/widget/PDirSelectWidget.h"

#include "ui/common/widget/PWidgetTitleWrapper.h"
#include "./material/PImageOptionWidget.h"
#include "./material/PHashOptionWidget2.h"

class CPOperationButtonContainer;
class CPImageWidgetBase : public QFrame
{
	Q_OBJECT
public:
	CPImageWidgetBase(int deviceTreeFlags, QWidget* parent);

	CPOperationButtonContainer* getButtonContainer() {
		return _buttonContainer;
	}
	CPFilePathContainer* getImagePath() {
		return _filePathContainer;
	}

	void setBeforeCase(bool beforeCase) {
		_beforeCase = beforeCase;
	}

	protected slots:
	virtual void slotStartButtonClicked() = 0;
	void slotOptionSelectChanged();

protected:
	void initCallback();
	virtual void getDeviceParam(const libdt::OneDevice&oneDevice, libdt::TDeviceParam& devParam) = 0;
	bool checkSelect(libdt::OneDevices& sourceDevices);
	bool checkTargetPath(devtask::ImageFormatOption::ImageType,
		uint64 totalSize, uint64 singleSize);

	static bool checkDestExist(const std::wstring& imagePath);

private:
	static std::wstring getImageTypeExt(devtask::ImageFormatOption::ImageType type);
	//检查路径后缀名
	static bool checkPathExt(devtask::ImageFormatOption::ImageType type,
		const std::wstring& imagePath);

protected:
	QVBoxLayout* _centerLayout;
	CPDeviceTreeWidget* _sourceTree;
	CPWidgetTitleWrapper* _sourceWrapper;
	CPImageOptionWidget* _imageOptionWidget;
	CPDirSelectWidget* _imageDirWidget;
	CPFileNameEdit* _filenameEdit;
	CPFilePathContainer* _filePathContainer;
	CPHashOptionWidget2* _hashWidget;

	CPOperationButtonContainer* _buttonContainer;
	bool _beforeCase;
};