#include "stdafx.h"
#include "PNetShareDialog.h"

#include "sidevice/PDeviceManager.h"
#include "sinetwork/PShareFolder.h"

#ifdef OS_PUNIX
const std::string G_SHARE_REG = "([/]{2}\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}[/]{1}[^/]+)"; //远程路径
#else
const std::string G_SHARE_REG = "([\\\\/]{2}\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}[\\\\/]{1}[^\\\\/]+)";
#endif
const QRegExp IpExp(QString::fromStdString(G_SHARE_REG));

CPNetShareDialog::CPNetShareDialog(QWidget* parent)
    : CPDialog2(parent)
{
    setWindowTitle(tr("share dir"));
    resize(QSize(300, 200));

    QWidget* container = new QWidget();

    QGridLayout* layout = new QGridLayout();

    QLabel* remoteDirTitle = new QLabel(tr("remote dir:"));
    _remoteDirEidt = new QLineEdit();
    QRegExpValidator *regValid = new QRegExpValidator(IpExp, this);
    _remoteDirEidt->setValidator(regValid);
    QString ipPlace;
#ifdef OS_PUNIX
    ipPlace = "//192.168.1.10/";
#else
    ipPlace = "\\\\192.168.1.10\\";
#endif
    ipPlace += tr("dir name");
    _remoteDirEidt->setPlaceholderText(ipPlace);
    QLabel* userTitle = new QLabel(tr("user:"));
    _userEidt = new QLineEdit();
    QLabel* passwdTitle = new QLabel(tr("password:"));
    _passwdEidt = new QLineEdit();
    _passwdEidt->setEchoMode(QLineEdit::Password);

    layout->addWidget(remoteDirTitle, 0, 0);
    layout->addWidget(_remoteDirEidt, 0, 1);
    layout->addWidget(userTitle, 1, 0);
    layout->addWidget(_userEidt, 1, 1);
    layout->addWidget(passwdTitle, 2, 0);
    layout->addWidget(_passwdEidt, 2, 1);

    container->setLayout(layout);
    setContentWidget(container);
	setupButtons();
}

bool CPNetShareDialog::checkInputs()
{
    if (_remoteDirEidt->text().trimmed().isEmpty()) {
        CPMessageBox::warning(tr("remote dir cann not empty!"));
        return false;
    }
#if 0
    if (_userEidt->text().trimmed().isEmpty()) {
        CPMessageBox::warning(tr("remote dir cann not empty!"));
        return false;
    }
    if (_passwdEidt->text().trimmed().isEmpty()) {
        CPMessageBox::warning(tr("remote dir cann not empty!"));
        return false;
    }
#endif
    return true;
}

void CPNetShareDialog::onOkClicked()
{
    if (!checkInputs()) {
        return;
    }
    QRegExp ipExp = IpExp;
    QString remoteDir = _remoteDirEidt->text().trimmed();
    if (!remoteDir.contains(ipExp)) {
        CPMessageBox::warning(tr("Invalid remote dir!"));
        return;
    }
    std::wstring remotePath = ipExp.cap(1).toStdWString();
	std::wstring userName = _userEidt->text().trimmed().toStdWString();
	std::wstring password = _passwdEidt->text().trimmed().toStdWString();

    std::wstring mntDir;
#ifdef OS_PUNIX
    using namespace pdevice;
    DevicePartition remotePartition;
    if (CPDeviceManager::get()->getPartitionByName(remotePath, remotePartition) &&
            !remotePartition.mountPoints.empty()) {
        //之前已mount
        mntDir = remotePartition.mountPoints.at(0);
    }
    else {
        //之前没有mount
        std::wstring dirName = CPPath::getFileName(remotePath);
        mntDir = CPPath::getNewPath(L"/mnt/" + dirName);
        if (!CPPath::createDirs(mntDir)) {
            CPMessageBox::error(tr("create mount point failed!"));
            return;
        }

		if (!net::CPShareFolder::mountShareFolder(remotePath, userName, password, mntDir)) {
			CPMessageBox::error(tr("mount share failed!"));
			return;
		}
	}
#elif OS_PWINDOWS
	if (!net::CPShareFolder::openShareFolder(remotePath, userName, password)) {
		CPMessageBox::error(tr("open share failed!"));
		return;
	}
	mntDir = remotePath;
#endif
    _mountDir = mntDir;
    CPDialog::onOkClicked();
}

std::wstring CPNetShareDialog::getMountDir()
{
    return _mountDir;
}
