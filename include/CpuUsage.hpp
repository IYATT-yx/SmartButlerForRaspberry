/**
 * @copyright (c) 2021 IYATT-yx
 * @file CpuUsage.hpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 2021-2-25
 */
#ifndef CPUUSAGE_HPP
#define CPUUSAGE_HPP

#include "InfoBase.hpp"
#include <fstream>

/**
 * @brief CPU使用率类
 */
class CpuUsage : public InfoBase
{
public:
    CpuUsage();
    std::string getInfo(std::vector<long> &outVec);
private:
    // 记录上一次读取的数据
    long m_user, m_nice, m_sys, m_idle, m_iowait, m_irq, m_softirq, m_total;
    // 用于对应文件的行格式读取内容,但是实际不用这个数据
    std::string m_cpuNULL;
    // 用于打开文件的实例
    std::ifstream m_readCPU;
};

#endif