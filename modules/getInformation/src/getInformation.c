/**
 * \file getInformation.c
 * \brief 获取信息 - CPU温度、CPU使用率、内存信息、IP信息
 * \author IYATT-yx
 * \copyright Copyright (C) 2021 IYATT-yx \n
 * 基于 AGPL-3.0 许可
 */
#include "getInformation.h"

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>

int getCPUTemp(void)
{
    FILE *stream = fopen("/sys/class/thermal/thermal_zone0/temp", "r");

    /**
     * 调试
     */
    #if DEBUG
        if (stream == NULL)
        {
            perror("getCPUTemp");
            exit(EXIT_FAILURE);
        }
    #endif

    char temS[7] = {'\0'};
    fread(temS, sizeof(char), sizeof(temS), stream);
    fclose(stream);

    long tem = 0;
    sscanf(temS, "%ld", &tem);

    return (int)(tem / 1000);
}

/**
 * 时间历史值记录
 */
static long lidle = 0, ltotal = 0;
int getCPUUsage(void)
{
    FILE *stream = fopen("/proc/stat", "r");

    /**
     * 调试
     */
    #if DEBUG
        if (stream == NULL)
        {
            perror("getCPUUsage");
            exit(EXIT_FAILURE);
        }
    #endif

    long user = 0, nice = 0, sys = 0, idle = 0, iowait = 0, irq = 0, softirq = 0, total = 0;
    char timeS[64];
    fread(timeS, sizeof(char), sizeof(timeS), stream);
    fclose(stream);

    sscanf(timeS, "%*s %ld %ld %ld %ld %ld %ld %ld", &user, &nice, &sys, &idle, &iowait, &irq, &softirq);
    total = user + nice + sys + idle + iowait + irq + softirq;

    int usage = (int)((double)(total - ltotal - (idle - lidle)) / (double)(total - ltotal) * 100);

    ltotal = total;
    lidle = idle;

    return usage;
}

static struct ifaddrs *ifAddrStruct = NULL;
static void *tmpAddrPtr = NULL;
void getIP(IP *ip)
{
    getifaddrs(&ifAddrStruct);
    while (ifAddrStruct != NULL)    
    {
        if (ifAddrStruct->ifa_addr->sa_family == AF_INET)
        {
            tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

            memcpy(ip->type, ifAddrStruct->ifa_name, strlen(ifAddrStruct->ifa_name));
            memcpy(ip->ip, addressBuffer, strlen(addressBuffer));
        }
        ifAddrStruct = ifAddrStruct->ifa_next;
    } 
}