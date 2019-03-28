#pragma once

class CPIpDialog : public CPDialog2
{
    Q_OBJECT
public:
    CPIpDialog(QWidget* parent = nullptr);

protected:
    void onOkClicked() override;

private:
    void initCardCombo();
    bool resetNetwork(const std::string& cardName);

    QComboBox* _cardBox;
    QLineEdit* _ipEdit;

};
