#ifndef __SYSTICK_H
#define __SYSTICK_H

/* ͷ�ļ� */
#include "gd32f4xx.h"

/* �������� */
void Systick_Init(uint16_t sysclk);
void delay_us(uint32_t n);
void delay_ms(uint32_t n);
void delay_nms(uint16_t n);

#endif
