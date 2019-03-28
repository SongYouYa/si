#pragma once


//含有取消按钮的输入框
class CPSearchEdit : public QWidget
{
	Q_OBJECT
public:
	CPSearchEdit(QWidget* parent = nullptr);

	QString text() {
		return _lineEdit->text();
	}
	void setText(const QString& text) {
		_lineEdit->setText(text);
	}
	void setPlaceholderText(const QString& text) {
		_lineEdit->setPlaceholderText(text);
	}
	//设置文件改变时，延时触发时间，默认500ms
	void setTimerInterval(int interval) {
		_timer->setInterval(interval);
	}

signals:
	void signalTextChanged(const QString&);
	void signalEnterPressed();
	void signalCancelClicked();

protected:
	void showEvent(QShowEvent* e) override;
	void keyPressEvent(QKeyEvent * event) override;

	private slots:
	void slotTextChanged(const QString& text);
	void slotCancelClicked();
	//延时发送文字变化消息
	void slotTimeout();

private:
	QLineEdit* _lineEdit;
	QLabel* _searchLabel;
	QPushButton* _cancelButton;

	QTimer * _timer;
};