#ifndef __LED_DRV_H
#define __LED_DRV_H

/* ͷ�ļ� */
#include "gd32f4xx.h"

/* LED�˿ڶ��� */
#define LED0(x)   do{ x ? \
                      gpio_bit_write(GPIOF, GPIO_PIN_9, SET) : \
                      gpio_bit_write(GPIOF, GPIO_PIN_9, RESET); \
                  }while(0)      

#define LED1(x)   do{ x ? \
                      gpio_bit_write(GPIOF, GPIO_PIN_10, SET) : \
                      gpio_bit_write(GPIOF, GPIO_PIN_10, RESET); \
                  }while(0)     
                  
/* LEDȡ������ */
#define LED0_TOGGLE()   do{gpio_bit_toggle (GPIOF, GPIO_PIN_9); }while(0)        /* ��תLED0 */
#define LED1_TOGGLE()   do{gpio_bit_toggle (GPIOF, GPIO_PIN_10); }while(0)       /* ��תLED1 */

/* �������� */
void LED_Init(void);

#endif
