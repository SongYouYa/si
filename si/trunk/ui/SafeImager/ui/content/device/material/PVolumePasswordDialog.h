#pragma once
#include "uicommon/dialog/PDialog.h"

class CPVolumePasswordDialog : public puic::CPDialog
{
    Q_OBJECT
public:
    CPVolumePasswordDialog(QWidget* parent = nullptr);

    QString password();

protected:
    void onOkClicked() override;
    void showEvent(QShowEvent* event) override;

private:
    QLineEdit* _lineEdit;
};
