#pragma once


//����ȡ����ť�������
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
	//�����ļ��ı�ʱ����ʱ����ʱ�䣬Ĭ��500ms
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
	//��ʱ�������ֱ仯��Ϣ
	void slotTimeout();

private:
	QLineEdit* _lineEdit;
	QLabel* _searchLabel;
	QPushButton* _cancelButton;

	QTimer * _timer;
};