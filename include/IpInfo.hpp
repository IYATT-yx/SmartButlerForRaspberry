/**
 * @copyright (c) 2021 IYATT-yx
 * @file IpInfo.hpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 2021-2-24
 * 
 * @note 如果获取 IP 失败 (未联网),如何处理
 */
#ifndef IPINFO_HPP
#define IPINFO_HPP

#include "InfoBase.hpp"
#include <string>

/**
 * @brief IP
 */
class IpInfo : public InfoBase
{
public:
    std::string getInfo(std::vector<long> &outVec);
};

#endif