/**
 * @copyright (c) 2021 IYATT-yx
 * @file main.cpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 2021-2-24
 */
#include "CpuTemp.hpp"
#include "IpInfo.hpp"
#include "CpuUsage.hpp"
#include "RamInfo.hpp"
#include "Oled.hpp"
extern "C"
{
    #include "wiringPiI2C.h"
    #include <sys/types.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <fcntl.h>
    #include <sys/file.h>
}
#include <iostream>
#include <cstdio>
#include <ctime>

int main()
{
    // 使用文件锁防止重复运行本程序
    int fileLock = open("/tmp/SmartButler.lock", O_CREAT | O_TRUNC);
    if (flock(fileLock, LOCK_NB | LOCK_EX) == -1)
    {
        std::cerr << "重复运行!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 向风扇传送温度数据的管道
    int fanPipe[2];
    // 向 RGB 传送温度数据的管道
    int rgbPipe[2];
    // 创建管道
    if (pipe(fanPipe) == -1 || pipe(rgbPipe) == -1)
    {
        exit(EXIT_FAILURE);
    }

    // 用于操作扩展板的句柄 - 初始化 - 需要传入扩展版的I2C地址 (使用 gpio i2cdetect 查询)
    int i2cFd = wiringPiI2CSetup(0x0D);

    // 创建两个子进程
    int pCounter;   // 进程计数
    for (pCounter = 0; pCounter < 2; ++pCounter)
    {
        // 禁止子进程再创建子进程
        if (fork() == 0)
        {
            break;
        }
    }

    // 第一个子进程控制风扇
    if (pCounter == 0)
    {
        // 读取温度数据,关闭写端
        close(fanPipe[1]);
        
        // 储存字符型字符串温度
        char tempChar[4];
        // 整数温度
        long cpuTempValue;
        // 记录上一次获取的CPU温度
        long lastCpuTempValue = 0;

        while (true)
        {
            // 从管道读取温度 - 读取失败就终止当前进程 (考虑在OLED显示错误的功能)
            if (read(fanPipe[0], tempChar, sizeof(tempChar)) <= 0)
            {
                exit(EXIT_FAILURE);
            }

            // 字符型温度转整数
            cpuTempValue = atol(tempChar);

            // 持续超温就启动风扇
            if (cpuTempValue >= 45 && lastCpuTempValue >= 45)
            {
                wiringPiI2CWriteReg8(i2cFd, 0x08, 0x01);
            }
            else
            {
                wiringPiI2CWriteReg8(i2cFd, 0x08, 0x00);
            }

            // 记录当次获得的温度数据
            lastCpuTempValue = cpuTempValue;
        }
    }
    // 第二个子进程控制 RGB - 预留,暂不考虑复杂设计
    else if (pCounter == 1)
    {
        // 读取温度数据,关闭写端
        close(rgbPipe[1]);

        exit(EXIT_SUCCESS);
    }
    // 父进程定时获取温度并发送给子进程
    else if (pCounter == 2)
    {
        // 温控进程未设计 , 暂时使用下面代码 (彩虹灯)
        wiringPiI2CWriteReg8(i2cFd, 0x04, 0x03);

        // 暂存系统时钟
        time_t now;
        // 字符串时间
        std::string timeStr;
        // 时间结构
        struct tm *timeStruct;


        // 父进程负责向三个管道写数据,则关闭读端
        close(fanPipe[0]);
        close(rgbPipe[0]);

        // 实例化 CPU温度类
        InfoBase *cpuTemp = new CpuTemp;
        // 实例化 IP类
        InfoBase *ipInfo = new IpInfo;
        // 实例化 CPU使用率类
        InfoBase *cpuUsage = new CpuUsage;
        // 实例化内存类
        InfoBase *ramInfo = new RamInfo;
        // 实例化oled
        Oled oled;

        oled.clear();
        oled.addString(2, 5, "Load......");
        oled.addString(17, 20, "iyatt.com");
        oled.show();


        // 储存字符型字符串温度
        char tempChar[4];
        // 传出参数
        std::vector<long> outVec;
        // 控制周期计数
        int counter = 0;
        // IP
        std::string ip;

        while (true)
        {
            sleep(1);
            oled.clear();
            // 时间显示闪烁
            if (counter % 2 == 0)
            {
                // 获取系统时间
                time(&now);
                // timeStr = ctime(&now);
                timeStruct = localtime(&now);
                timeStr = std::to_string(timeStruct->tm_hour) + " : " + std::to_string(timeStruct->tm_min);
                oled.addString(48, 32, timeStr);
            }

            // 获取 CPU温度
            cpuTemp->getInfo(outVec);
            // long 转字符型字符串
            sprintf(tempChar, "%ld", outVec[0]);
            // 向子进程发送温度数据
            write(fanPipe[1], tempChar, sizeof(tempChar));
            write(rgbPipe[1], tempChar, sizeof(tempChar));
            oled.addString(0, 0, "T= " + std::to_string(outVec[0]) + "C");
            // 清空
            outVec.clear();


            // 获取 CPU使用率
            cpuUsage->getInfo(outVec);
            oled.addString(64, 0, "CPU: " + std::to_string(outVec[0]) + "%");
            // 清空
            outVec.clear();


            // 获取内存和交换分区信息
            ramInfo->getInfo(outVec);
            oled.addString(0, 8, "RAM: " + std::to_string(outVec[1]) + "/" + std::to_string(outVec[0]) + "MB");
            // 清空
            outVec.clear();

            if (counter == 1)
            {
                // 获取IP
                ip = ipInfo->getInfo(outVec);
                // 清空
                outVec.clear();
            }
            ++counter;
            // 控制周期, 3秒
            if (counter == 3)
            {
                counter = 0;
            }
            // 无网络连接时,提示
            if (ip.empty())
            {
                oled.addString(0, 16, "Network: None");
            }
            else
            {
                oled.addString(0, 16, ip);
            }

            oled.show();
        }
    }
}