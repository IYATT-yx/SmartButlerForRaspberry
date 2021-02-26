/**
 * @copyright (c) 2021 IYATT-yx
 * @file CpuUsage.cpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 2021-2-25
 */
#include "CpuUsage.hpp"
extern "C"
{
    #include <unistd.h>
}

CpuUsage::CpuUsage()
{
    // 实例化时,读取用于第一次调用时计算CPU使用率
    m_readCPU.open("/proc/stat", std::ifstream::in);
    m_readCPU >> m_cpuNULL >> m_user >> m_nice >> m_sys >> m_idle >> m_iowait >> m_irq >> m_softirq;
    m_total = m_user + m_nice + m_sys + m_idle + m_iowait + m_irq + m_softirq;
    m_readCPU.close();
}

/**
 * @brief 获取CPU使用率
 * @param outVec 下标0传出CPU使用率
 * @return 空
 */
std::string CpuUsage::getInfo(std::vector<long> &outVec)
{
    // 打开文件
    m_readCPU.open("/proc/stat", std::ifstream::in);
    // 用户态运行时间 nice值为负的进程所用时间 处于核心态的运行时间 除IO的时间 IO时间 硬中断时间 软中断时间
    long user, nice, sys, idle, iowait, irq, softirq, total;
    // 读取数据
    m_readCPU >> m_cpuNULL >> user >> nice >> sys >> idle >> iowait >> irq >> softirq;
    // 关闭文件
    m_readCPU.close();
    // 计算总时间
    total = user + nice + sys + idle + iowait + irq + softirq;

    // 计算并传出CPU使用率: (总时间 - idle) / 总时间
    outVec.push_back(static_cast<long>(static_cast<double>(total - m_total - (idle - m_idle)) / static_cast<double>(total - m_total) * 100));

    // 记录本次读取的数据,用于下次读取后,计算差值得到这个时间段内的时间
    m_user = user;
    m_nice = nice;
    m_sys = sys;
    m_idle = idle;
    m_iowait = iowait;
    m_irq = irq;
    m_softirq = softirq;
    m_total = total;

    return "";
}