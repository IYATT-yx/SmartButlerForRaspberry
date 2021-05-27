/**
 * \file main.c
 * \brief 扩展版控制主程序
 * \author IYATT-yx
 * \copyright Copyright (C) 2021 IYATT-yx \n
 * 基于 AGPL-3.0 许可
 */
#include "oled.h"
#include "getInformation.h"
#include "wiringPiI2C.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

#if DEBUG
    #include <time.h>
#endif

int main()
{
    /**
     * 防重复运行，文件锁
     */
    int fileLock = open("/tmp/SmartButlerForRaspberry.lock", O_CREAT | O_TRUNC);
    if (flock(fileLock, LOCK_NB | LOCK_EX) == -1)
    {
        #if DEBUG
            printf("只允许一个运行的实例\n");
        #endif

        exit(EXIT_FAILURE);
    }


    static int tem; // 温度值
    static int usage;   // 使用率值
    static IP ip;   // IP 信息结构
    static char usageS[12] = {'\0'};    // 使用率段显示内容
    static char temS[10] = {'\0'};  // 温度段显示内容
    static char ipS[24] = {'\0'};   // IP 信息段显示内容

    #if DEBUG
        static time_t now;
        static struct tm *tm;
        static char timeS[20];
    #endif

    /**
     * 初始化扩展板
     */
    int fd = wiringPiI2CSetup(0x0D);

    /**
     * LED 灯效：彩虹灯模式
     */
    wiringPiI2CWriteReg8(fd, 0x04, 0x03);

    /**
     * 初始化 oled
     */
    oledInit();

    while (1)
    {
        /**
         * 获取温度，使用率，IP信息
         */
        tem = getCPUTemp();
        usage = getCPUUsage();
        getIP(&ip);

        /**
         * 温度达到 50 摄氏度启动风扇
         */
        if (tem > 50)
        {
            wiringPiI2CWriteReg8(fd, 0x08, 0x01);
        }
        else
        {
            wiringPiI2CWriteReg8(fd, 0x08, 0x00);
        }

        sprintf(usageS, "Usage: %d%%", usage);
        sprintf(temS, "Tem: %d", tem);
        sprintf(ipS, "%s: %s", ip.type, ip.ip);

        /**
         * 上一版存在运行一段时间停止的情况
         * 调试版本加入时间显示，当程序停止运行后，oled 没有信号刷新就会停止到那个时间
         */
        #if DEBUG
            time(&now);
            tm = localtime(&now);

            sprintf(timeS, "%d-%d-%d %d:%d:%d", tm->tm_year + 1900, tm->tm_mon, tm->tm_mday
                                                , tm->tm_hour, tm->tm_min, tm->tm_sec);

            printf("%s\n", usageS);
            printf("%s\n", temS);
            printf("%s\n", ipS);
            printf("%s\n", timeS);

            putText(1, 34, timeS);
        #endif

        putText(1, 1, usageS);
        putText(1, 12, temS);
        putText(1, 23, ipS);

        show();
        clear();

        /**
         * 刷新周期 4s
         */
        sleep(4);
    }
}