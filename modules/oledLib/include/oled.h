/**
 * \file oled.h
 * \brief oled 操作库
 * \author IYATT-yx
 * \copyright Copyright (C) 2021 IYATT-yx \n
 * 基于 AGPL-3.0 许可
 */
#ifndef OLED_H
#define OLED_H

/**
 * oled 分辨率
 */
#define WIDTH   128
#define HEIGHT  64

#define VCCSTATE    0x02

/**
 * oled 的 i2c 地址
 */
#define ADDR    0x3c


/**
 * 初始化 oled
 */
extern void oledInit(void);

/**
 * \brief 在根据 oled 分辨率生成的二位数组缓存中指定位置刷入指定的字符串
 * \param x 自左边线往右的坐标值
 * \param y 自上边线往下的坐标值
 * \param s 需要显示的字符串
 */
extern void putText(int x, int y, const char *s);

/**
 * 将缓存中数据刷新到 oled 屏
 */
extern void show(void);

/**
 * 清空缓存
 */
extern void clear(void);


#endif