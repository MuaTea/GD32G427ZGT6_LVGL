#ifndef __GT1151Q_H
#define __GT1151Q_H	


#include <stdint.h>

/* ����ݴ��������ӵ�оƬ����(δ����IIC����) 
 * IO�������� 
 */
#define GT_RST(x)    		 do{ x ? \
                             gpio_bit_write(GPIOC, GPIO_PIN_13, SET) : \
                             gpio_bit_write(GPIOC, GPIO_PIN_13, RESET); \
                         }while(0) 	    /* GT1151��λ���� */

#define GT_INT    		   gpio_input_bit_get(GPIOB, GPIO_PIN_1)		/* GT1151�ж����� */	
   	
 
/* IIC��д���� */	
#define GT_CMD_WR 		0X28     	/* д���� */
#define GT_CMD_RD 		0X29		  /* ������ */
  
/* GT1151 ���ּĴ������� */ 
#define GT_CTRL_REG 	0X8040   	/* GT1151���ƼĴ��� */
#define GT_PID_REG 		0X8140   	/* GT1151��ƷID�Ĵ��� */

#define GT_GSTID_REG 	0X814E   	/* GT1151��ǰ��⵽�Ĵ������ */
#define GT_TP1_REG 		0X8150  	/* ��һ�����������ݵ�ַ */
#define GT_TP2_REG 		0X8158		/* �ڶ������������ݵ�ַ */
#define GT_TP3_REG 		0X8160		/* ���������������ݵ�ַ */
#define GT_TP4_REG 		0X8168		/* ���ĸ����������ݵ�ַ */
#define GT_TP5_REG 		0X8170		/* ��������������ݵ�ַ */  
 
 
uint8_t gt1151_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len);  /* ��GT1151д������ */
void gt1151_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len);     /* ��GT1151��ȡ���� */
uint8_t gt1151_init(void);               /* GT1151��������ʼ������ */
uint8_t gt1151_scan(uint8_t mode);       /* ������ɨ�躯�� */

#endif











