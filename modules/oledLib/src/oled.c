/**
 * \file oled.c
 * \brief oled 操作库
 * \author IYATT-yx
 * \copyright Copyright (C) 2021 IYATT-yx \n
 * 基于 AGPL-3.0 许可
 */
#include "font.h"
#include "oled.h"

#include "wiringPiI2C.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ************************************************************************************
// 内部

/**
 * oled 屏幕缓冲区
 */
static int oledBuffer[1024] = {0};

/**
 * oled 句柄
 */
static int oledFd;

/**
 * \brief 发送命令
 * \param cmd 命令值
 */
static void command(int cmd);

/**
 * \brief 绘制像素点
 * \param x 自左边线往右的坐标值
 * \param y 自上边线往下的坐标值
 */
static void drawPixel(int x, int y);

/**
 * \brief 显示字符
 * \param x 自左边线往右的坐标值
 * \param y 自上边线往下的坐标值
 * \param ch 要显示的字符
 */
static void drawChar(int x, int y, int ch);
// 内部
// ************************************************************************************

void oledInit(void)
{
    /**
     * 初始化 oled I2C
     */
    oledFd = wiringPiI2CSetup(ADDR);

    /**
     * 调试
     */
    #if DEBUG
        if (oledFd == -1)
        {
            printf("%s: wiringPiI2CSetup 初始化失败", __func__);
            exit(EXIT_FAILURE);
        }
    #endif

    command(0xAE);
    command(0xD5);
    command(0x80);
    command(0xA8);
    command(HEIGHT - 1);
    command(0xD3);
    command(0x00);
    command(0x40 | 0x00);
    command(0x8D);
    command(0x14);
    command(0x20);
    command(0x00);
    command(0xA0 | 0x01);
    command(0xC8);
    command(0xDA);
    command(0x12);
    command(0x81);
    command(0xCF);
    command(0xD9);
    command(0xF1);
    command(0xDB);
    command(0x40);
    command(0xA4);
    command(0xA6);
    command(0x2E);
    command(0xAF);
}

void putText(int x, int y, const char *s)
{
    /**
     * 调试
     */
    #if DEBUG
        if ((x < 1) || (x > WIDTH) || (y < 1) || (y > HEIGHT))
        {
            printf("%s: 坐标越界\n", __func__);
            exit(EXIT_FAILURE);
        }
    #endif

	int point_x = x;
	int point_y = y;

	for (size_t i = 0; i < strlen(s); ++i)
	{
		if (s[i] == '\n')
		{
			point_y += 1 * 8;
			point_x = 0;
		}
		else if (s[i] == '\r')
		{
			;
		}
		else
		{
			drawChar(point_x, point_y, s[i]);
			point_x += 1 * 6;
			if (1 && (point_x > (WIDTH - 1 * 6)))
			{
				point_y += 1 * 8;
				point_x = 0;
			}
		}
	}  
}

void show(void)
{
    command(0x21);
    command(0x00);
    command(WIDTH - 1);

    command(0x22);
    command(0x00);
    command(0x07);

	for (int i = 0; i < (WIDTH * HEIGHT / 8); ++i)
    {
		wiringPiI2CWriteReg8(oledFd, 0x40, oledBuffer[i]); 
	}
}

void clear(void)
{
    memset(oledBuffer, 0, sizeof(oledBuffer));
}

void command(int cmd)
{
    wiringPiI2CWriteReg8(oledFd, 0x00, cmd);
}

void drawPixel(int x, int y)
{
    /**
     * 调试
     */
    #if DEBUG
        if ((x < 1) || (x > WIDTH) || (y < 1) || (y > HEIGHT))
        {
            printf("%s: 坐标越界\n", __func__);
            exit(EXIT_FAILURE);
        }
    #endif

    oledBuffer[x + (y / 8) * WIDTH] |= (1 << (y & 7));
}

void drawChar(int x, int y, int ch)
{
    /**
     * 调试
     */
    #if DEBUG
        if ((x >= WIDTH) || (y >= HEIGHT) || (x < 1) || (y < 1))
        {
            printf("%s: 坐标越界\n", __func__);
            exit(EXIT_FAILURE);
        }
    #endif


	for (int i = 0; i < 6; ++i)
    {
		int line;
		if (i == 5)
        {
			line = 0x0;
        }
		else
        {
			line = (* (const unsigned char *)(font + (ch * 5) + i));
        }

		for (int j = 0; j < 8; j++)
        {
			if (line & 0x1)
            {
					drawPixel(x + i, y + j);
			}
			line >>= 1;
		}
	}
}