#pragma once
#include "uicommon/progress/PLoadingWidgetBase.h"

namespace puic
{
	class CPLineLoadingWidget : public CPLoadingWidgetBase
	{
		Q_OBJECT
	public:
		CPLineLoadingWidget(QWidget* parent);
		void setLoading(bool loading) override;

	protected:
		bool eventFilter(QObject * watched, QEvent * event) override;

	private:
		void updateAnimate();

		QWidget* _parent;
		QPropertyAnimation* _animation;
	};
}
