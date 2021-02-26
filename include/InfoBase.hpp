/**
 * @copyright (c) 2021 IYATT-yx
 * @file InfoBase.hpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 2021-2-24
 */
#ifndef INFO_HPP
#define INFO_HPP

#include <string>
#include <vector>

/**
 * @brief 功能接口类 - 基类
 */
class InfoBase
{
public:
    /**
     * @brief 纯虚函数 - 派生类根据需要重写
     * @param outVec 根据需要可以传出一个或多个数据
     * @return 如果需要传出的信息为字符串,则接函数收返回值
     */
    virtual std::string getInfo(std::vector<long> &outVec) = 0;
};


#endif