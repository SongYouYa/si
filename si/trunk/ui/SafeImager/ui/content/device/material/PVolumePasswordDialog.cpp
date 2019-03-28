#include "stdafx.h"
#include "PVolumePasswordDialog.h"



CPVolumePasswordDialog::CPVolumePasswordDialog(QWidget *parent)
    : CPDialog(parent)
{
    resize(QSize(250, 170));

    QWidget* content = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();

    QLabel* title = new QLabel(tr("volume password:"));
    _lineEdit = new QLineEdit();
    _lineEdit->setEchoMode(QLineEdit::Password);

    layout->addWidget(title);
    layout->addWidget(_lineEdit);
    content->setLayout(layout);

    setContentWidget(content);
}

void CPVolumePasswordDialog::showEvent(QShowEvent *event)
{
    CPDialog::showEvent(event);
    _lineEdit->setFocus(Qt::ActiveWindowFocusReason);
}

QString CPVolumePasswordDialog::password()
{
    return _lineEdit->text();
}

void CPVolumePasswordDialog::onOkClicked()
{
    QString strPassword = _lineEdit->text();
    if (strPassword.isEmpty()) {
        CPMessageBox::info(tr("Please input password."));
        return;
    }
    CPDialog::onOkClicked();
}
