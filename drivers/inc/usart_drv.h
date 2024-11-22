#ifndef __USART_DRV_H
#define __USART_DRV_H

/* ͷ�ļ� */
#include "gd32f4xx.h"
#include <stdio.h>
#include "stdarg.h"
#include "string.h"

/* �궨�� */
#define USART_REC_LEN               200         /* �����������ֽ��� 200 */
#define USART1_TXBUFF_SIZE   256
#define USART_EN_RX                 1

/* �������� */
void usart_init(uint32_t bound);

#endif
