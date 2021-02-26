/**
 * @copyright (c) 2021 IYATT-yx
 * @file CpuTemp.hpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 2021-2-24
 */
#include "CpuTemp.hpp"
#include <fstream>

/**
 * @brief 获取CPU温度
 * @param outVec 下标0传出温度
 * @return 空
 */
std::string CpuTemp::getInfo(std::vector<long> &outVec)
{
    // 打开文件
    std::ifstream cpuTempStream("/sys/class/thermal/thermal_zone0/temp", std::ifstream::in);
    // 获取温度
    cpuTempStream >> m_cpuTemp;
    // 关闭文件
    cpuTempStream.close();
    // 传出温度 (摄氏度)
    outVec.push_back(m_cpuTemp / 1000);

    return "";
}