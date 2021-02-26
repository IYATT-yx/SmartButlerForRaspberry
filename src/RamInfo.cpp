/**
 * @copyright (c) 2021 IYATT-yx
 * @file RamInfo.cpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 1021-2-26
 */
#include "RamInfo.hpp"

/**
 * @brief 获取RAM信息和交换分区使用信息
 * @param outVec 下标: 0 总内存; 1 可用内存;
 * @return 空
 */
std::string RamInfo::getInfo(std::vector<long> &outVec)
{
    m_readMem.open("/proc/meminfo", std::ifstream::in);

    // 总内存
    while (m_item != "MemTotal:")
    {
        m_readMem >> m_item >> m_value >> m_strNull;
    }
    outVec.push_back(static_cast<long>(m_value >> 10));

    // 可用内存
    while (m_item != "MemAvailable:")
    {
        m_readMem >> m_item >> m_value >> m_strNull;
    }
    outVec.push_back(static_cast<long>(m_value >> 10));

    m_readMem.close();
    return "";
}