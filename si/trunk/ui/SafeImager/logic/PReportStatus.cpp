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
		_dirty = true;  //����true
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
			return ReportUpdated; //�����ѷ����ı�
		}
		else {
			return ReportDone; //����������
		}
	}
	else {
		return ReportUndone; //�ľ���û�����ɱ���
	}
}

void CPReportStatus::sendRecordChanged()
{
	emit signalRecordChanged();
}