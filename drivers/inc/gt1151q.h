#ifndef __GT1151Q_H
#define __GT1151Q_H	


#include <stdint.h>

/* 与电容触摸屏连接的芯片引脚(未包含IIC引脚) 
 * IO操作函数 
 */
#define GT_RST(x)    		 do{ x ? \
                             gpio_bit_write(GPIOC, GPIO_PIN_13, SET) : \
                             gpio_bit_write(GPIOC, GPIO_PIN_13, RESET); \
                         }while(0) 	    /* GT1151复位引脚 */

#define GT_INT    		   gpio_input_bit_get(GPIOB, GPIO_PIN_1)		/* GT1151中断引脚 */	
   	
 
/* IIC读写命令 */	
#define GT_CMD_WR 		0X28     	/* 写命令 */
#define GT_CMD_RD 		0X29		  /* 读命令 */
  
/* GT1151 部分寄存器定义 */ 
#define GT_CTRL_REG 	0X8040   	/* GT1151控制寄存器 */
#define GT_PID_REG 		0X8140   	/* GT1151产品ID寄存器 */

#define GT_GSTID_REG 	0X814E   	/* GT1151当前检测到的触摸情况 */
#define GT_TP1_REG 		0X8150  	/* 第一个触摸点数据地址 */
#define GT_TP2_REG 		0X8158		/* 第二个触摸点数据地址 */
#define GT_TP3_REG 		0X8160		/* 第三个触摸点数据地址 */
#define GT_TP4_REG 		0X8168		/* 第四个触摸点数据地址 */
#define GT_TP5_REG 		0X8170		/* 第五个触摸点数据地址 */  
 
 
uint8_t gt1151_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len);  /* 向GT1151写入数据 */
void gt1151_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len);     /* 从GT1151读取数据 */
uint8_t gt1151_init(void);               /* GT1151触摸屏初始化函数 */
uint8_t gt1151_scan(uint8_t mode);       /* 触摸屏扫描函数 */

#endif











