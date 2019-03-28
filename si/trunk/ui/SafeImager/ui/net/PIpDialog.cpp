#include "stdafx.h"
#include "PIpDialog.h"

#include "sinetwork/PNetwork.h"
#include "sicommon/PProcess.h"
using namespace net;

const QString G_IP_REGEX = "(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})"; //ip addr


CPIpDialog::CPIpDialog(QWidget *parent)
    : CPDialog2(parent)
{
    setWindowTitle(tr("ip set"));
    setCancelButtonText(tr("skip"));

    resize(QSize(300, 150));

    QWidget* container = new QWidget();

    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(10, 20, 10, 15);
    layout->setSpacing(15);

    QLabel* cardTitle = new QLabel(tr("netcard:"));
    _cardBox = new QComboBox();
    _cardBox->setObjectName("netcard");
    layout->addWidget(cardTitle, 0, 0);
    layout->addWidget(_cardBox, 0, 1);

    QLabel* ipTitle = new QLabel(tr("local ip addr:"));
    _ipEdit = new QLineEdit();
    _ipEdit->setPlaceholderText(tr("same segment with remote pc"));
    QRegExp rx(G_IP_REGEX); //限制非法字符
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    _ipEdit->setValidator(pReg);
    layout->addWidget(ipTitle, 1, 0);
    layout->addWidget(_ipEdit, 1, 1);

    container->setLayout(layout);
    setContentWidget(container);
	setupButtons();

    initCardCombo();
}

void CPIpDialog::initCardCombo()
{
    std::vector<std::string> cardNames;
    CPNetwork::getNetCards(cardNames);
    QString curText;
    for(auto& name : cardNames) {
        QString cardName = QString::fromStdString(name);
        if (cardName.contains("wlp")) {
            //过滤掉无线网卡
            continue;
        }
        QString display = cardName + " (";
        if (cardName.contains("enp")) {
            display += tr("lan card");
            curText = display;
        }
        display += ")";
        _cardBox->addItem(display, cardName);
    }
    _cardBox->setCurrentText(curText);
}

bool CPIpDialog::resetNetwork(const std::string &cardName)
{
    std::fstream stream;
    stream.open("/etc/network/interfaces", std::ios::out | std::ios::app);
    if (!stream.is_open()) {
        return false;
    }
    std::string staticConfig = "\niface " + cardName + " inet static";
    stream << staticConfig << std::endl;
    stream.close();

    std::wstring resetCmd = L"/etc/init.d/network-manager restart";
    int exitCode = 0;
    if (!CPProcess::createProcess(resetCmd, L"", NULL, &exitCode) ||
            exitCode != 0) {
        return false;
    }

    return true;
}

void CPIpDialog::onOkClicked()
{
    if (_cardBox->count() == 0) {
        CPMessageBox::warning(tr("No net card!"));
        return;
    }
    std::string cardName = _cardBox->currentData().toString().toStdString();
    if (_ipEdit->text().trimmed().isEmpty()) {
        CPMessageBox::warning(tr("ip can not empty!"));
        return;
    }
    QRegExp ipExp(G_IP_REGEX);
    QString strIp = _ipEdit->text().trimmed();
    if (!strIp.contains(ipExp)) {
        CPMessageBox::warning(tr("Invalid ip addr!"));
        return;
    }

    resetNetwork(cardName);

    std::string ip = ipExp.cap(1).toStdString();
    bool ret = net::CPNetwork::setIp(cardName, ip, "255.255.255.0");
    if (!ret) {
        CPMessageBox::error(tr("Set ip failed!"));
        return;
    }
    CPDialog::onOkClicked();
}
