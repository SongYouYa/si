#pragma once

class CPNetShareDialog : public CPDialog2
{
    Q_OBJECT
public:
    CPNetShareDialog(QWidget* parent = nullptr);
    std::wstring getMountDir();

protected:
    void onOkClicked() override;

    bool checkInputs();

    QLineEdit* _remoteDirEidt;
    QLineEdit* _userEidt;
    QLineEdit* _passwdEidt;

    std::wstring _mountDir;
};
