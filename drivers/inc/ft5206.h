#ifndef __FT5206_H
#define __FT5206_H	

#include "stdint.h"

/* ����ݴ��������ӵ�оƬ����(δ����IIC����) 
 * IO�������� 
 */
#define FT_RST(x)    			do{ x ? \
                              gpio_bit_write(GPIOC, GPIO_PIN_13, SET) : \
                              gpio_bit_write(GPIOC, GPIO_PIN_13, RESET); \
                          }while(0)	  /* FT5206��λ���� */

#define FT_INT    			 	gpio_input_bit_get(GPIOB,GPIO_PIN_1)   /* FT5206�ж�����	 */

/* IIC��д���� */	
#define FT_CMD_WR 				     0X70       /* д����(���λΪ0) */
#define FT_CMD_RD 				     0X71		    /* ������(���λΪ1) */
  
/* FT5206 ���ּĴ������� */ 
#define FT_DEVIDE_MODE 			   0x00   	  /* FT5206ģʽ���ƼĴ��� */
#define FT_REG_NUM_FINGER      0x02		    /* ����״̬�Ĵ��� */

#define FT_TP1_REG 				     0X03	  	  /* ��һ�����������ݵ�ַ */
#define FT_TP2_REG 				     0X09		    /* �ڶ������������ݵ�ַ */
#define FT_TP3_REG 				     0X0F		    /* ���������������ݵ�ַ */
#define FT_TP4_REG 				     0X15		    /* ���ĸ����������ݵ�ַ */
#define FT_TP5_REG 				     0X1B		    /* ��������������ݵ�ַ */ 
 

#define	FT_ID_G_LIB_VERSION		 0xA1		    /* �汾 */		
#define FT_ID_G_MODE 			     0xA4   		/* FT5206�ж�ģʽ���ƼĴ��� */
#define FT_ID_G_THGROUP			   0x80   		/* ������Чֵ���üĴ��� */
#define FT_ID_G_PERIODACTIVE   0x88   		/* ����״̬�������üĴ��� */


uint8_t ft5206_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len);  /* ��FT5206д������ */
void ft5206_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len);     /* ��FT5206��ȡ���� */
uint8_t ft5206_init(void);                                       /* FT5206��������ʼ������ */
uint8_t ft5206_scan(uint8_t mode);                               /* ������ɨ�躯�� */

#endif

















