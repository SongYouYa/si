#pragma once


class CPSplitterCollapser : public QObject
{
	Q_OBJECT
public:
	CPSplitterCollapser(QObject* parent = nullptr);

	struct PRect
	{
		int minWidth;
		int maxWidth;
		PRect()
			: minWidth(0), maxWidth(0)
		{}
	};
	void setSplitter(QSplitter* splitter, const PRect& firstRect);

	public slots:
	void slotToggleCollapse();

private:
	QSplitter* _splitter;
	PRect _rect;

	bool _doing;
};
