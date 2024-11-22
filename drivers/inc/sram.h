#ifndef __SRAM_H
#define __SRAM_H

#include "stdint.h"


#define SRAM_EXMC_NEX           2           /* 使用EXMC_NE2接SRAM_CS,取值范围只能是: 0~3 */

/* SRAM基地址, 根据 SRAM_EXMC_NEX 的设置来决定基址地址
 * 我们一般使用EXMC的块0(BANK0)来驱动SRAM, 块0地址范围总大小为256MB,均分成4块:
 * 存储块0(EXMC_NE0)地址范围: 0X6000 0000 ~ 0X63FF FFFF
 * 存储块1(EXMC_NE1)地址范围: 0X6400 0000 ~ 0X67FF FFFF
 * 存储块2(EXMC_NE2)地址范围: 0X6800 0000 ~ 0X6BFF FFFF
 * 存储块3(EXMC_NE3)地址范围: 0X6C00 0000 ~ 0X6FFF FFFF
 */
#define SRAM_BASE_ADDR         (0X60000000 + (0X4000000 * SRAM_EXMC_NEX))


void sram_init(void);   /* SRAM 初始化 */
void sram_write(uint8_t *pbuf, uint32_t addr, uint32_t datalen);    /* SRAM 写入 */
void sram_read(uint8_t *pbuf, uint32_t addr, uint32_t datalen);     /* SRAM 读取 */

uint8_t sram_test_read(uint32_t addr);
void sram_test_write(uint32_t addr, uint8_t data);

#endif

















