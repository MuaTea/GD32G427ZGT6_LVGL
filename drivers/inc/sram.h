#ifndef __SRAM_H
#define __SRAM_H

#include "stdint.h"


#define SRAM_EXMC_NEX           2           /* ʹ��EXMC_NE2��SRAM_CS,ȡֵ��Χֻ����: 0~3 */

/* SRAM����ַ, ���� SRAM_EXMC_NEX ��������������ַ��ַ
 * ����һ��ʹ��EXMC�Ŀ�0(BANK0)������SRAM, ��0��ַ��Χ�ܴ�СΪ256MB,���ֳ�4��:
 * �洢��0(EXMC_NE0)��ַ��Χ: 0X6000 0000 ~ 0X63FF FFFF
 * �洢��1(EXMC_NE1)��ַ��Χ: 0X6400 0000 ~ 0X67FF FFFF
 * �洢��2(EXMC_NE2)��ַ��Χ: 0X6800 0000 ~ 0X6BFF FFFF
 * �洢��3(EXMC_NE3)��ַ��Χ: 0X6C00 0000 ~ 0X6FFF FFFF
 */
#define SRAM_BASE_ADDR         (0X60000000 + (0X4000000 * SRAM_EXMC_NEX))


void sram_init(void);   /* SRAM ��ʼ�� */
void sram_write(uint8_t *pbuf, uint32_t addr, uint32_t datalen);    /* SRAM д�� */
void sram_read(uint8_t *pbuf, uint32_t addr, uint32_t datalen);     /* SRAM ��ȡ */

uint8_t sram_test_read(uint32_t addr);
void sram_test_write(uint32_t addr, uint8_t data);

#endif

















