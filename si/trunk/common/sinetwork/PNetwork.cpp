#include "stdafx.h"
#include "sinetwork/PNetwork.h"

#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include "boost/regex.hpp"
#include "sicommon/PProcess.h"

namespace net
{

void CPNetwork::getNetCards(std::vector<std::string> &cardNames)
{
    const boost::regex reg("^([\\w]+):\\s+.*");
    std::string content;
    int exitCode = 0;
    if (!CPProcess::createProcess(L"cat /proc/net/dev", L"", &content, &exitCode) ||
            exitCode != 0) {
        return;
    }
    std::vector<std::string> lines;
    CPString::split(content, '\n', lines);
    for(auto& line : lines) {
        boost::smatch what;
        if (!boost::regex_match(line, what, reg)) {
            continue;
        }
        std::string cardName = what[1];
        cardNames.push_back(cardName);
    }
}

void CPNetwork::getNetInfos(NetcardInfos &netInfos)
{
    struct ifaddrs* ifAddrs = NULL;
    getifaddrs(&ifAddrs);

    struct ifaddrs* curAddr = ifAddrs;
    void* tmpAddrPtr = NULL;
    for (; curAddr != NULL; curAddr = curAddr->ifa_next) {
        if (curAddr->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            if (std::string(curAddr->ifa_name) == "lo") {
                continue;
            }
            NetcardInfo netInfo;
             netInfo.cardName = curAddr->ifa_name;
            //ip
            tmpAddrPtr=&((struct sockaddr_in *)curAddr->ifa_addr)->sin_addr;
            char ipBuf[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, ipBuf, INET_ADDRSTRLEN);
            if (std::string(ipBuf) == "127.0.0.1") {
                continue;
            }
            netInfo.ipv4 = ipBuf;
            //netmask
            tmpAddrPtr=&((struct sockaddr_in *)curAddr->ifa_netmask)->sin_addr;
            inet_ntop(AF_INET, tmpAddrPtr, ipBuf, INET_ADDRSTRLEN);
            netInfo.netmask = ipBuf;

            netInfos.push_back(netInfo);
        }
    }
    freeifaddrs(ifAddrs);
}

bool CPNetwork::setIp(const std::string &cardName, const std::string &ip, const std::string &mask)
{
#ifdef __linux__
    if (cardName.empty() || ip.empty()) {
        return false;
    }
    if (cardName.size() > IF_NAMESIZE) {
        return false;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return false;
    }

    //ip
    struct sockaddr_in addrIn;
    memset(&addrIn, 0, sizeof(addrIn));

    struct ifreq ifrq;
    memset(&ifrq, 0, sizeof(ifrq));
    strncpy(ifrq.ifr_ifrn.ifrn_name, cardName.c_str(), IF_NAMESIZE);

    addrIn.sin_family = AF_INET;
    addrIn.sin_addr.s_addr = inet_addr(ip.c_str());
    memcpy(&ifrq.ifr_ifru.ifru_addr, &addrIn, sizeof(addrIn));
    int ret = ioctl(sock, SIOCSIFADDR, &ifrq);
    if (ret < 0) {
        close(sock);
        return false;
    }

    //设置激活标志
    ifrq.ifr_ifru.ifru_flags |= IFF_UP | IFF_RUNNING;
    ret = ioctl(sock, SIOCSIFFLAGS, &ifrq);
    if (ret < 0) {
        close(sock);
        return false;
    }

    //netmask
    memset(&ifrq, 0, sizeof(ifrq));
    strncpy(ifrq.ifr_ifrn.ifrn_name, cardName.c_str(), IF_NAMESIZE);
    addrIn.sin_family = AF_INET;
    addrIn.sin_addr.s_addr = inet_addr(mask.c_str());
    memcpy(&ifrq.ifr_ifru.ifru_addr, &addrIn, sizeof(addrIn));
    ret = ioctl(sock, SIOCSIFNETMASK, &ifrq);
     if (ret < 0) {
        int error = errno;
        return false;
    }

    close(sock);
    return true;
#else
    return false;
#endif
}

}
