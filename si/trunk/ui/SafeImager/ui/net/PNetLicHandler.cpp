#include "stdafx.h"
#include "PNetLicHandler.h"
#include "PNetLicenseDialog.h"

#ifdef OS_PUNIX
#include "NetLic/LinuxDefs.h"
#endif
#include "NetLic/Common/Logger.h"
#include "NetLic/Common/PMessages.h"
#include "NetLic/Client/PAuthClient.h"
#include "NetLic/Client/PAuthClientHandler.h"

#include "PLicDefs.h"

using namespace auth;

typedef std::shared_ptr<CPAuthClient> CPAuthClientPtr;
typedef std::shared_ptr<CPAuthClientHandler> CPAuthClientHandlerPtr;

class CPNetLicHandler::Impl
{
public:
	CPAuthClientPtr _client;
	CPAuthClientHandlerPtr _clientHandler;
};


CPNetLicHandler* CPNetLicHandler::_instance = nullptr;

CPNetLicHandler::CPNetLicHandler(QObject* parent)
	: QObject(parent),
	_impl(new Impl()),
	_isAuthed(false),
	_port(0)
{
#ifndef OS_PAPPLE
	static StdConsoleLogger flw;
	Logger::Get().SetLogWriter(&flw);
	Logger::Get().SetLevel(Logger::LEVEL_INFO);
#endif

#ifdef OS_PWINDOWS
	WSADATA wsdata;
	WSAStartup(0x0202, &wsdata);
#endif

	connect(this, SIGNAL(signalKeepAlive()), this,
		SLOT(slotKeepAlive()), Qt::QueuedConnection);
	connect(this, SIGNAL(signalAliveResult(bool)), this, 
		SLOT(slotAliveResult(bool)), Qt::QueuedConnection);
}

CPNetLicHandler::~CPNetLicHandler()
{
	if (_impl) {
		delete _impl;
		_impl = nullptr;
	}
}

CPNetLicHandler* CPNetLicHandler::get()
{
	if (!_instance) {
		_instance = new CPNetLicHandler();
	}
	return _instance;
}

bool CPNetLicHandler::isAuthed()
{
	return _isAuthed;
}

int CPNetLicHandler::applyAuth(int port)
{
#ifndef OS_PAPPLE
	PLOG_DEBUG(L"apply auth.");
	_port = port;
	_impl->_client = std::make_shared<CPAuthClient>(port);
	_impl->_clientHandler = std::make_shared<CPAuthClientHandler>(_impl->_client.get());
	//产品ID
	_impl->_clientHandler->SetProduct(std::atoi(PPRODUCTID));
	_impl->_client->SetHandler(_impl->_clientHandler.get());
	int rv = _impl->_client->ApplyAuth();
	if (rv == CPAuthClient::ACR_AUTH_APPLY_APPROVED) {
		_isAuthed = true;
		emit signalKeepAlive();
	}
	else {
		_isAuthed = false;
	}
	return rv;
#else
    return -1;
#endif
}

void CPNetLicHandler::slotKeepAlive()
{
	std::thread t(&CPNetLicHandler::keepAliveProc, this);
	t.detach();
}

void CPNetLicHandler::keepAliveProc()
{
#ifndef OS_PAPPLE
	while (true) {
		if (!_impl->_client->KeepAlive()) {
			//连接断开后，再重新申请一次
			int ret = applyAuth(_port);
			if (CPAuthClient::ACR_AUTH_APPLY_APPROVED != ret) {
				PLOG_DEBUG(L"retry apply auth failed.");
				_isAuthed = false;
				emit signalAliveResult(false);
			}
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
#endif
}

void CPNetLicHandler::slotAliveResult(bool ret)
{
	CPMessageBox::warning(tr("Net lic is disconnected, please apply again."));
	CPNetLicenseDialog licDialog;
	licDialog.setPort(_port);
	licDialog.exec();
}
