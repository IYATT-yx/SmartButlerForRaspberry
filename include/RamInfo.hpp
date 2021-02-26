/**
 * @copyright (c) 2021 IYATT-yx
 * @file RamInfo.hpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 2021-2-26
 */
#ifndef RAMINFO_HPP
#define RAMINFO_HPP

#include "InfoBase.hpp"
#include <fstream>

/**
 * @brief 内存类
 */
class RamInfo : public InfoBase
{
public:
    std::string getInfo(std::vector<long> &outVec);
private:
    // 暂存读取的值 (MB)
    long m_value;
    // 匹配项目名字
    std::string m_item;
    // 仅填充行读取格式 (获取单位),无用
    std::string m_strNull;
    // 读取文件
    std::ifstream m_readMem;
};

#endif