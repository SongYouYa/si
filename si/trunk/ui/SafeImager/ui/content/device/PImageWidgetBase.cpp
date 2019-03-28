#include "stdafx.h"
#include "PImageWidgetBase.h"
#include "ui/common/POperationButtonContainer.h"
#include "ui/common/POperationButton.h"
#include "ui/common/widget/PDirSelectWidget.h"
#include "ui/common/PFilePathChecker.h"

#include "uicommon/device/PDeviceUiUtils.h"

#include "libdevtask/PDeviceTaskUtils.h"

using namespace devtask;
using namespace pdevice;
using namespace libdt;

extern CPTabManager* g_mainTab;

CPImageWidgetBase::CPImageWidgetBase(int deviceTreeFlags, QWidget* parent)
	: QFrame(parent),
	_beforeCase(false)
{
	setObjectName("content_container");
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setMargin(0);

	_centerLayout = new QVBoxLayout();
	_centerLayout->setSpacing(10);
	_centerLayout->setContentsMargins(10, 5, 10, 10);

	QSplitter* splitter = new QSplitter();		//切分窗口
	splitter->setChildrenCollapsible(false);	//用户不能手动将子控件大小调为0.
	splitter->setOrientation(Qt::Vertical);		//垂直

	_sourceTree = new CPDeviceTreeWidget(deviceTreeFlags, this);
	std::vector<int> widths = {100, 200, 180};
	_sourceTree->setColumnWidths(widths);
	_sourceWrapper = new CPWidgetTitleWrapper(_sourceTree);
	_sourceWrapper->setTitle(tr("source device"));
	splitter->addWidget(_sourceWrapper);
	_centerLayout->addWidget(splitter);

	_filePathContainer = new CPFilePathContainer(this);
	_imageDirWidget = _filePathContainer->getDirWidget();
	_filenameEdit = _filePathContainer->getNameWidget();
	_centerLayout->addWidget(_filePathContainer);

	//_centerLayout->addWidget(new CPHSplitLine(this));

	_imageOptionWidget = new CPImageOptionWidget();
	auto optionWrapper = new CPWidgetTitleWrapper(_imageOptionWidget);
	optionWrapper->setTitle(tr("image option"));
	_centerLayout->addWidget(optionWrapper);

	//_centerLayout->addWidget(new CPHSplitLine(this));

	_hashWidget = new CPHashOptionWidget2();
	auto hashWrapper = new CPWidgetTitleWrapper(_hashWidget);
	hashWrapper->setTitle(tr("hash calc"));
	_centerLayout->addWidget(hashWrapper);


	layout->addLayout(_centerLayout);

	_buttonContainer = new CPOperationButtonContainer();
	auto operButton = CPOperationButton::newForensicButton();
	_buttonContainer->addRightWidget(operButton);
	layout->addWidget(_buttonContainer);

	setLayout(layout);

	connect(operButton, SIGNAL(clicked()), this, SLOT(slotStartButtonClicked()));
	connect(_sourceTree, SIGNAL(signalDeviceSelectChanged()), 
		this, SLOT(slotOptionSelectChanged()));
	connect(_imageOptionWidget, SIGNAL(signalFormatOptionChanged()), 
		this, SLOT(slotOptionSelectChanged()));
}

void CPImageWidgetBase::initCallback()
{
	if (!_beforeCase) {
		//内存镜像可能在创建案件之前，所以这里判断一下
		CPCase::get()->addCreateCallback([this](){
			std::wstring imageDir = CPCase::get()->caseDir() + L"/Images/";
			this->_imageDirWidget->setDefaultDir(imageDir);
		});
	}
}

void CPImageWidgetBase::slotOptionSelectChanged()
{
	libdt::OneDevices sourceDevices;
	_sourceTree->getSelectedDevices(sourceDevices);//获取选择目标磁盘
	if (sourceDevices.empty()) {
		return;
	}
	libdt::OneDevice sourceDevice = sourceDevices.at(0);
	libdt::TDeviceParam devParam;
	getDeviceParam(sourceDevice, devParam);

    std::wstring imageExt;
    //mac apfs卷不能直接镜像，需要打包文件
    bool fileZip = false;
#ifdef OS_PAPPLE
    if (sourceDevice.isPartition) {
        DevicePartition partition;
        CPDeviceManager::get()->getPartitionByName(sourceDevice.deviceName, partition);
        if (partition.partitionType == PPARTITION_APFS_VOLUME) {
            fileZip = true;
        }
    }
#endif
    if (fileZip) {
        _imageOptionWidget->setDisabled(true);
        _imageOptionWidget->setImageType(devtask::ImageFormatOption::IMAGE_ZIP);
    }
    else {
        _imageOptionWidget->setDisabled(false);
    }
    ImageFormatOption formatOptions;
    _imageOptionWidget->getOptions(formatOptions);
    imageExt = getImageTypeExt(formatOptions.imageType);

    std::wstring imageName = devParam.deviceDisplayName + imageExt;
	boost::replace_all(imageName, L":", L"");
	_filenameEdit->setText(QString::fromStdWString(imageName));
}

bool CPImageWidgetBase::checkSelect(libdt::OneDevices& sourceDevices)
{
	_sourceTree->getSelectedDevices(sourceDevices);
	if (sourceDevices.empty()) {
		CPMessageBox::warning(tr("Please select source devices."));
		return false;
	}
	return true;
}

bool CPImageWidgetBase::checkTargetPath(devtask::ImageFormatOption::ImageType type,
	uint64 totalSize, uint64 singleSize)
{
	if (_imageDirWidget->getPath().trimmed().isEmpty()) {
		CPMessageBox::info(tr("Please select out dir."));
		_imageDirWidget->setFocus(Qt::ActiveWindowFocusReason);
		return false;
	}
	if (_filenameEdit->text().trimmed().isEmpty()) {
		CPMessageBox::info(tr("Please input file name."));
		_filenameEdit->setFocus(Qt::ActiveWindowFocusReason);
		return false;
	}
	std::wstring imagePath = _filePathContainer->getPath();
	if (!checkPathExt(type, imagePath)) {
		//补全后缀名
		CPFileNameEdit* nameEdit = _filePathContainer->getNameWidget();
		std::wstring strExt = getImageTypeExt(type);
		nameEdit->setText(nameEdit->text().trimmed() + QString::fromStdWString(strExt));
		imagePath = _filePathContainer->getPath();
	}
	if (!CPFilePathChecker::checkTargetPath(imagePath, totalSize, singleSize)) {
		return false;
	}
	//最后再检测并创建目标目录，防止目录创建在本地磁盘里
	if (!_imageDirWidget->createDirs()) {
		return false;
	}
	return true;
}

bool CPImageWidgetBase::checkDestExist(const std::wstring& imagePath)
{
	QFile f(QString::fromStdWString(imagePath));
	if (f.exists()) {
		int ret = CPMessageBox::question(tr("Destination exits, overwrite?"));
		if (ret == QDialog::Accepted) {
			if (!f.remove()) {
				CPMessageBox::error(tr("Overwrite destination failed!"));
				return false;
			}
			return true;
		}
		else {
			return false;
		}
	}
	return true;
}

std::wstring CPImageWidgetBase::getImageTypeExt(devtask::ImageFormatOption::ImageType type)
{
	if (type == ImageFormatOption::IMAGE_DD) {
		return L".dd";
	}
	else if (type == ImageFormatOption::IMAGE_E01) {
		return L".e01";
	}
	else if (type == ImageFormatOption::IMAGE_AFF) {
		return L".aff";
	}
    else if (type == ImageFormatOption::IMAGE_ZIP) {
        return L".zip";
    }
	return L"";
}

bool CPImageWidgetBase::checkPathExt(devtask::ImageFormatOption::ImageType type,
	const std::wstring& imagePath)
{
	std::wstring ext = getImageTypeExt(type);
	std::wstring pathExt = boost::filesystem::path(imagePath).extension().wstring();
	if (!CPString::isEquali(ext, pathExt)) {
		return false;
	}
	return true;
}
