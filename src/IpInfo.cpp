/**
 * @copyright (c) 2021 IYATT-yx
 * @file IpInfo.cpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 2021-2-24
 */
#include "IpInfo.hpp"
extern "C"
{
    #include <arpa/inet.h>
    #include <ifaddrs.h>
    #include <netinet/in.h>
}
#include <cstring>

/**
 * @brief 获取IP信息
 * @param outVec 空
 * @return IP信息
 */
std::string IpInfo::getInfo(std::vector<long> &outVec)
{
    // 不使用传出参数
    (void)outVec;

    struct ifaddrs *ifAddrStruct = NULL;
    void *tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);
    while (ifAddrStruct != NULL)    
    {
        if (ifAddrStruct->ifa_addr->sa_family == AF_INET)
        {
            tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

            if (strcmp(ifAddrStruct->ifa_name, "eth0") == 0)
            {
                return "eth" + std::to_string(0) + ": " + addressBuffer;
            }
            else if (strcmp(ifAddrStruct->ifa_name, "wlan0") == 0)
            {
                return "wlan" + std::to_string(0) + ": " + addressBuffer;
            }
        }
        ifAddrStruct = ifAddrStruct->ifa_next;
    }

    return "";
}