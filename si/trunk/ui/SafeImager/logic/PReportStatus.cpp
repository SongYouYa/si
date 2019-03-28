#include "stdafx.h"
#include "PReportStatus.h"

CPReportStatus*CPReportStatus::_instance = nullptr;

CPReportStatus::CPReportStatus(QObject* parent)
    : QObject(parent), _created(false), _dirty(false)
{

}

CPReportStatus *CPReportStatus::get(QObject* parent)
{
	if (!_instance) {
		_instance = new CPReportStatus(parent);
	}
	return _instance;
}

void CPReportStatus::setRecordDirty()
{	
	if(_created){
		_dirty = true;  //返回true
	}
}

void CPReportStatus::setReportCreated()
{
	_created = true;
	_dirty = false;
}

int CPReportStatus::getStatus()
{
	if (_created) {
		if (_dirty) {
			return ReportUpdated; //报告已发生改变
		}
		else {
			return ReportDone; //报告以生成
		}
	}
	else {
		return ReportUndone; //的就是没有生成报告
	}
}

void CPReportStatus::sendRecordChanged()
{
	emit signalRecordChanged();
}