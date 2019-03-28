#pragma once

class CPOpenPathButton : public QPushButton
{
	Q_OBJECT
public:
	CPOpenPathButton(const std::wstring& openPath, QWidget* parent = nullptr);

private slots:
	void showPathWidget();

private:
	std::wstring _imagePath;
};