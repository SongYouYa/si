#include "stdafx.h"
#include "PNetLicenseDialog.h"
#include "PNetLicHandler.h"

#ifdef OS_PUNIX
#include "NetLic/LinuxDefs.h"
#endif
#include "NetLic/Client/PAuthClient.h"


CPNetLicenseDialog::CPNetLicenseDialog(QWidget* parent)
	: CPDialog2(parent)
{
	setWindowTitle(tr("net auth"));

	resize(QSize(300, 180));

	QWidget* container = new QWidget();

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(10, 20, 10, 15);
	layout->setSpacing(20);

	QHBoxLayout* licLayout = new QHBoxLayout();
	licLayout->setSpacing(5);
	QLabel* licTitle = new QLabel(tr("net auth num:"));
	_licEdit = new QLineEdit();
	QRegExp rx("([0-9]+)"); //限制非法字符
	QRegExpValidator *pReg = new QRegExpValidator(rx, this);
	_licEdit->setValidator(pReg);
	licLayout->addWidget(licTitle);
	licLayout->addWidget(_licEdit);
	layout->addLayout(licLayout);

	QHBoxLayout* loadLayout = new QHBoxLayout();
	loadLayout->addStretch();
	_loadingWidget = new CPLoadingWidget();
	_loadingWidget->setVisible(false);
	loadLayout->addWidget(_loadingWidget);
	_msgLabel = new QLabel();
	_msgLabel->setStyleSheet("color: red;");
	_msgLabel->setVisible(false);
	loadLayout->addWidget(_msgLabel);
	loadLayout->addStretch();

	layout->addLayout(loadLayout);

	layout->addStretch();
	container->setLayout(layout);

	setContentWidget(container);
	setupButtons();

	connect(this, SIGNAL(signalApplyResult(int)), this, SLOT(slotApplyReslut(int)),
		Qt::QueuedConnection);

	CPNetLicHandler::get(); //在UI线程初始化handler实例
}

void CPNetLicenseDialog::onOkClicked()
{
	QString strPort = _licEdit->text().trimmed();
	if (strPort.isEmpty()) {
		CPMessageBox::warning(tr("auth num can not be empty!"));
		return;
	}
	_msgLabel->setVisible(false);
	_loadingWidget->setVisible(true);
	_loadingWidget->start();
	int port = strPort.toInt();
	std::thread t([port, this](){
		int ret = CPNetLicHandler::get()->applyAuth(port);
		emit signalApplyResult(ret);
	});
	t.detach();
}

void CPNetLicenseDialog::slotApplyReslut(int ret)
{
	_loadingWidget->setVisible(false);
	_loadingWidget->stop();

	_msgLabel->setVisible(true);
	QString msg;
	using namespace auth;
	if (CPAuthClient::ACR_AUTH_APPLY_APPROVED == ret) {
		//success
		CPDialog::onOkClicked();
		return;
	}
	else if (CPAuthClient::ACR_AUTH_NO_MORE_LICENSE == ret) {
		msg = tr("no more lic!");
	}
	else if (CPAuthClient::ARC_AUTH_INTERNAL_ERROR == ret) {
		msg = tr("server inter error!");
	}
	else if (CPAuthClient::ARC_AUTH_SERVER_DISCONNECTED == ret) {
		msg = tr("server disconnect!");
	}
	else if (CPAuthClient::ARC_AUTH_USE_TIME_EXPIRED == ret) {
		msg = tr("use time expired!");
	}
	else if (CPAuthClient::ARC_AUTH_SERVICE_TIME_EXPIRED == ret) {
		msg = tr("service time expired!");
	}
	else {
		msg = tr("apply auth failed!");
	}
	_msgLabel->setText(msg);
}

void CPNetLicenseDialog::closeEvent(QCloseEvent * e)
{
	int ret = CPMessageBox::question(tr("Sure to quit the program?"));
	if (ret == CPDialog::Accepted) {
		QApplication::exit(0);
	}
	else {
		e->ignore();
		return;
	}
	CPDialog::closeEvent(e);
}

void CPNetLicenseDialog::setPort(int port)
{
	_licEdit->setText(QString::number(port));
}
