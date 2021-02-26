/**
 * @copyright (c) 2021 IYATT-yx
 * @file CpuTemp.hpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 2021-2-24
 */
#ifndef CPUTEMP_HPP
#define CPUTEMP_HPP

#include "InfoBase.hpp"

/**
 * @brief CPU温度类
 */
class CpuTemp : public InfoBase
{
public:
    std::string getInfo(std::vector<long> &outVec);

private:
    // 保存读取温度的源数据 (毫摄氏度)
    long m_cpuTemp;
};

#endif