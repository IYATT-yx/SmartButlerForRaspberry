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
}
#include <iostream>
#include <cstdio>

int main()
{
    // 向风扇传送温度数据的管道
    int fanPipe[2];
    // 向 RGB 传送温度数据的管道
    int rgbPipe[2];
    // 向 oled 传送温度数据的管道
    int oledPipe[2];
    // 创建管道
    if (pipe(fanPipe) == -1 || pipe(rgbPipe) == -1 || pipe(oledPipe) == -1)
    {
        exit(EXIT_FAILURE);
    }

    // 用于操作扩展板的句柄 - 初始化 - 需要传入扩展版的I2C地址 (使用 gpio i2cdetect 查询)
    int i2cFd = wiringPiI2CSetup(0x0D);

    // 创建三个子进程
    int pCounter;   // 进程计数
    for (pCounter = 0; pCounter < 3; ++pCounter)
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

            // 调式 - 输出当前温度
            std::cout << "风扇进程: 当前温度: " << cpuTempValue << " 上一次温度: " << lastCpuTempValue << std::endl;

            // 持续超温就启动风扇
            if (cpuTempValue >= 50 && lastCpuTempValue >= 50)
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
    // 第三个子进程控制 oled
    else if (pCounter == 2)
    {
        // 读取温度数据,关闭写端
        close(oledPipe[1]);

        exit(EXIT_SUCCESS);
    }
    // 父进程定时获取温度并发送给子进程
    else if (pCounter == 3)
    {
        // 父进程负责向三个管道写数据,则关闭读端
        close(fanPipe[0]);
        close(rgbPipe[0]);
        close(oledPipe[0]);

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


        // 储存字符型字符串温度
        char tempChar[4];
        // 传出参数
        std::vector<long> outVec;
        // 控制周期计数 - 某些信息不需要频繁获取
        int counter = 0;
        // IP
        std::string ip;

        while (true)
        {
            sleep(2);

            // 获取 CPU温度
            cpuTemp->getInfo(outVec);
            // long 转字符型字符串
            sprintf(tempChar, "%ld", outVec[0]);
            // 向子进程发送温度数据
            write(fanPipe[1], tempChar, sizeof(tempChar));
            write(rgbPipe[1], tempChar, sizeof(tempChar));
            write(oledPipe[1], tempChar, sizeof(tempChar));
            // 调试 - CPU温度显示
            std::cout << "父进程: CPU温度: " << outVec[0] << std::endl;
            oled.clear();
            oled.addString(0, 0, "T= " + std::to_string(outVec[0]) + "C");
            // 清空
            outVec.clear();


            // 获取 CPU使用率
            cpuUsage->getInfo(outVec);
            // 调试 - CPU使用率
            std::cout << "父进程: CPU使用率: " << outVec[0] << std::endl;
            oled.addString(64, 0, "CPU: " + std::to_string(outVec[0]) + "%");
            // 清空
            outVec.clear();


            // 获取内存和交换分区信息
            ramInfo->getInfo(outVec);
            // 调试 - 终端输出
            std::cout << "总内存: " << outVec[0] << " 可用内存: " << outVec[1] << std::endl;
            oled.addString(0, 8, "RAM: " + std::to_string(outVec[1]) + "/" + std::to_string(outVec[0]) + "MB");
            // 清空
            outVec.clear();

            if (counter == 0)
            {
                // 获取IP
                ip = ipInfo->getInfo(outVec);
                // 调试 - 终端输出
                std::cout << "父进程: " << ip << std::endl;
                // 清空
                outVec.clear();
            }
            ++counter;
            // 控制周期, 2s * 15 = 30s
            if (counter == 15)
            {
                counter = 0;
            }
            oled.addString(0, 16, ip);

            oled.show();
            std::cout << std::string(50, '+') << std::endl;

            // 温控进程未设计 , 暂时使用下面代码 (彩虹灯)
            wiringPiI2CWriteReg8(i2cFd, 0x04, 0x03);
        }
    }
}