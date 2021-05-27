/**
 * \file getInformation.h
 * \brief 获取信息 - CPU温度、CPU使用率、内存信息、IP信息
 * \author IYATT-yx
 * \copyright Copyright (C) 2021 IYATT-yx \n
 * 基于 AGPL-3.0 许可
 */
#ifndef GETINFORMATION_H
#define GETINFORMATION_H

/**
 * IP 信息数据结构
 */
typedef struct
{
    /**
     * 网卡类型：
     *  eth0 有线
     *  wlan0 无线
     */
    char type[6];
    /**
     * IP
     * xxx.xxx.xxx.xxx
     */
    char ip[16];
} IP;

/**
 * \brief 获取 CPU 温度
 * \return CPU 温度（单位：摄氏度）
 */
extern int getCPUTemp(void);

/**
 * \brief 获取 CPU 使用率
 * \return CPU 使用率值 %
 */
extern int getCPUUsage(void);

/**
 * \brief 获取 IP 信息
 * \param ip 传入 IP 数据结构类型的变量
 */
extern void getIP(IP *ip);

#endif