#include "stdafx.h"
#include "PCaseHandler.h"
#include "PContextHandler.h"

#include "ui/content/case/PCaseInfoDialog.h"

#include "controller/PCaseController.h"


bool CPCaseHandler::checkCaseCreated()
{
    bool promptCase = true; //是否提示创建案件
    if (CPCase::caseRootDir().empty()) {
        CPMessageBox::info(tr("Please set out dir and create case first."));
        CPContextHandler::initContext();
        promptCase = false;
    }
    if (CPCase::caseRootDir().empty()) {
        return false;
    }

	std::wstring caseNumber = CPCase::get()->caseNumber();
	if (!caseNumber.empty()) {
		return true;
	}
	
    if (promptCase) {
        CPMessageBox::info(tr("Please create case first."));
    }
	int ret = CPCaseInfoDialog::get(true)->exec();
	if (ret != CPDialog::Accepted) {
		return false;
	}
    return true;
}

bool CPCaseHandler::openCase(QWidget *parent)
{
    std::wstring caseNumber = CPCase::get()->caseNumber();
    if (caseNumber.empty()) {
        std::wstring runningCase = CPCaseController::get()->getRunningCase();
        if (!runningCase.empty() &&
            CPDialog::Accepted == CPMessageBox::question(
            QObject::tr("%1 has unfinished tasks, will you open the case?").arg(
            QString::fromStdWString(runningCase)))) {
            if (!CPCase::get()->creatCase(runningCase, false)) {
                CPMessageBox::error(QObject::tr("open case error!"));
                return false;
            }
            return true;
        }
        else {
            CPCaseController::get()->setRunningCase(L"");
			int ret = CPCaseInfoDialog::get(true, parent)->exec();
            if (ret == CPDialog::Accepted) {
                return true;
            }
        }
    }
    else {
		int ret = CPCaseInfoDialog::get(false, parent)->exec();
        return ret == CPDialog::Accepted;
    }

    return false;
}
