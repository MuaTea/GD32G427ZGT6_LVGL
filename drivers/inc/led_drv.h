#ifndef __LED_DRV_H
#define __LED_DRV_H

/* 头文件 */
#include "gd32f4xx.h"

/* LED端口定义 */
#define LED0(x)   do{ x ? \
                      gpio_bit_write(GPIOF, GPIO_PIN_9, SET) : \
                      gpio_bit_write(GPIOF, GPIO_PIN_9, RESET); \
                  }while(0)      

#define LED1(x)   do{ x ? \
                      gpio_bit_write(GPIOF, GPIO_PIN_10, SET) : \
                      gpio_bit_write(GPIOF, GPIO_PIN_10, RESET); \
                  }while(0)     
                  
/* LED取反定义 */
#define LED0_TOGGLE()   do{gpio_bit_toggle (GPIOF, GPIO_PIN_9); }while(0)        /* 翻转LED0 */
#define LED1_TOGGLE()   do{gpio_bit_toggle (GPIOF, GPIO_PIN_10); }while(0)       /* 翻转LED1 */

/* 函数声明 */
void LED_Init(void);

#endif
