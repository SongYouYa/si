#include "stdafx.h"

#include "sinetwork/PNetwork.h"
#include "sinetwork/PShareFolder.h"

#pragma comment(lib, "sinetwork.lib")

void test_net_linux()
{
	net::NetcardInfos netInfos;
	net::CPNetwork::getNetInfos(netInfos);
	//"10.0.2.15", 10.0.2.255
	bool ret = net::CPNetwork::setIp("enp0s3", "10.0.2.15", "255.255.255.0"); //"10.0.2.15", "192.168.1.101"
}

void test_win_share()
{
	using namespace net;
	bool ret = CPShareFolder::openShareFolder(L"\\\\192.168.16.178\\share2", L"pansafe", L"pansafe");
}

void test_network()
{
	test_win_share();
}