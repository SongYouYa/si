#include "stdafx.h"
#include "PDynamicTranslator.h"

CPDynamicTranslator::CPDynamicTranslator(QObject* parent)
{
	QT_TR_NOOP("tab_copy");
	QT_TR_NOOP("tab_filesystem");
	QT_TR_NOOP("tab_app");
	QT_TR_NOOP("tab_task");
	QT_TR_NOOP("tab_report");

	QT_TR_NOOP("device_tab_clone");
	QT_TR_NOOP("device_tab_image");
	QT_TR_NOOP("device_tab_memory");
	QT_TR_NOOP("device_tab_erase");

	QT_TR_NOOP("help_tab_image_product");
	QT_TR_NOOP("help_tab_file_filter");
	QT_TR_NOOP("help_tab_data_analyze");
	QT_TR_NOOP("help_tab_generate_report");
	QT_TR_NOOP("help_tab_close_task");
	QT_TR_NOOP("help_tab_contact_us");
	QT_TR_NOOP("help_tab_about");

	QT_TR_NOOP("tab_task_doing");
	QT_TR_NOOP("tab_task_done");
}

QString dtr(const QString& str)
{
	static CPDynamicTranslator translator;
	return translator.tr(str.toUtf8());
}
