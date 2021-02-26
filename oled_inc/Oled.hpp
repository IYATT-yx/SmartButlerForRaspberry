/**
* @copyright (c) 2021 IYATT-yx
 * @file Oled.hpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 2021-2-26
 */
#ifndef OLED_HPP
#define OLED_HPP

#include <string>

/**
 * @brief Oled类 - 驱动
 * @details 0.96寸 128x64 SSD1306 I2C
 */
class Oled
{
public:
    Oled();
    void addString(int x, int y, std::string str);
    void show();
    void clear();
private:
    //
    int m_vccState;
    // oled的i2c地址
    int m_oledAddr;
    // oled句柄
    int m_oledFd;
    // oled屏分辨率
    int m_width, m_height;
    // 缓冲区
    int m_buffer[1024];

    
    void _command(int cmd);
    void _drawPixel(int x, int y);
    void _drawChar(int x, int y, int ch);
};

#endif