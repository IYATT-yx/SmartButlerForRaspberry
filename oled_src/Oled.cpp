/**
 * @copyright (c) 2021 IYATT-yx
 * @file Oled.cpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 2021-2-26
 */
#include "Oled.hpp"
#include "font.hpp"
extern "C"
{
    #include "wiringPiI2C.h"
}
#include <iostream>
#include <cstring>

Oled::Oled()
{
    // 初始化分辨率
    this->m_width = 128;
    this->m_height = 64;

    // 初始化缓冲区
    memset(&this->m_buffer, 0, sizeof(this->m_buffer));

    this->m_vccState = 0x02;
    this->m_oledAddr = 0x3C;

    // 初始化oled
    m_oledFd = wiringPiI2CSetup(this->m_oledAddr);
    if (m_oledFd == -1)
    {
        std::cerr << "初始化oled失败!" << std::endl;
        exit(EXIT_FAILURE);
    }

    this->_command(0xAE);
    this->_command(0xD5);
    this->_command(0x80);

    this->_command(0xA8);
    this->_command(this->m_height - 1);

    this->_command(0xD3);
    this->_command(0x00);
    this->_command(0x40 | 0x00);
    this->_command(0x8D);

    this->_command(0x14);

    this->_command(0x20);
    this->_command(0x00);
    this->_command(0xA0 | 0x01);
    this->_command(0xC8);

    this->_command(0xDA);
    this->_command(0x12);
    this->_command(0x81);

    this->_command(0xCF);

    this->_command(0xD9);

    this->_command(0xF1);

    this->_command(0xDB);
    this->_command(0x40);
    this->_command(0xA4);
    this->_command(0xA6);

    this->_command(0x2E);

    this->_command(0xAF);
}


void Oled::addString(int x, int y, std::string str)
{
	int point_x = x;
	int point_y = y;

	for (auto i : str)
	{
		if (i == '\n')
		{
			point_y += 1 * 8;
			point_x = 0;
		}
		else if (i == '\r')
		{
			;
		}
		else
		{
			this->_drawChar(point_x, point_y, i);
			point_x += 1 * 6;
			if (1 && (point_x > (this->m_width - 1 * 6)))
			{
				point_y += 1 * 8;
				point_x = 0;
			}
		}
	}
}

void Oled::show()
{
    this->_command(0x21);
    this->_command(0x00);
    this->_command(this->m_width - 1);

    this->_command(0x22);
    this->_command(0x00);
    this->_command(0x07);

	for (int i = 0; i < (this->m_width * this->m_height / 8); ++i)
    {
		wiringPiI2CWriteReg8(this->m_oledFd, 0x40, m_buffer[i]); 
	}
}

void Oled::clear()
{
    memset(&this->m_buffer, 0, sizeof(this->m_buffer));
}


void Oled::_command(int cmd)
{
    wiringPiI2CWriteReg8(this->m_oledFd, 0x00, cmd);
}

/**
 * @brief 像素点绘制
 * @param x 横坐标
 * @param y 纵坐标
 */
void Oled::_drawPixel(int x, int y)
{
	if ((x < 0) || (x >= this->m_width) || (y < 0) || (y >= this->m_height))
	{
		std::cerr << "绘制像素点超出屏幕分辨率!" << std::endl;
		exit(EXIT_FAILURE);
	}

    m_buffer[x + (y / 8) * this->m_width] |= (1 << (y & 7));
}

/**
 * @brief 绘制字符
 * @param x 横坐标
 * @param y 纵坐标
 * @param ch 字符 
 */
void Oled::_drawChar(int x, int y, int ch)
{
	if ((x >= this->m_width) ||	(y >= this->m_height) || ((x + 5) < 0) || ((y + 7) < 0))
    {
        std::cerr << "绘制字符超出分辨率!" << std::endl;
        std::cout << this->m_width << " " << this->m_height << std::endl;
        exit(EXIT_FAILURE);
    }
	for (int i = 0; i < 6; ++i)
    {
		int line;
		if (i == 5)
        {
			line = 0x0;
        }
		else
        {
			line = (* static_cast<const unsigned char *>(font + (ch * 5) + i));
        }

		for (int j = 0; j < 8; j++)
        {
			if (line & 0x1)
            {
					this->_drawPixel(x + i, y + j);
			}
			line >>= 1;
		}
	}
}