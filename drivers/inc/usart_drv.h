#ifndef __USART_DRV_H
#define __USART_DRV_H

/* 头文件 */
#include "gd32f4xx.h"
#include <stdio.h>
#include "stdarg.h"
#include "string.h"

/* 宏定义 */
#define USART_REC_LEN               200         /* 定义最大接收字节数 200 */
#define USART1_TXBUFF_SIZE   256
#define USART_EN_RX                 1

/* 函数声明 */
void usart_init(uint32_t bound);

#endif
