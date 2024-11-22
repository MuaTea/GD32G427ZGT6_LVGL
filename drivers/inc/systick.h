#ifndef __SYSTICK_H
#define __SYSTICK_H

/* 头文件 */
#include "gd32f4xx.h"

/* 函数声明 */
void Systick_Init(uint16_t sysclk);
void delay_us(uint32_t n);
void delay_ms(uint32_t n);
void delay_nms(uint16_t n);

#endif
