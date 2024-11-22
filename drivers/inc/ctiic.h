#ifndef __CTIIC_H
#define __CTIIC_H
  
#include <stdint.h>

/* IO�������� */ 
#define CT_IIC_SCL(x)    do{ x ? \
                             gpio_bit_write(GPIOB, GPIO_PIN_0, SET) : \
                             gpio_bit_write(GPIOB, GPIO_PIN_0, RESET); \
                         }while(0)         /* SCL */ 

#define CT_IIC_SDA(x)    do{ x ? \
                             gpio_bit_write(GPIOF, GPIO_PIN_11, SET) : \
                             gpio_bit_write(GPIOF, GPIO_PIN_11, RESET); \
                         }while(0)         /* SDA */

#define CT_READ_SDA      gpio_input_bit_get(GPIOF, GPIO_PIN_11)  			/* ��ȡSDA */ 
 

/* IIC���в������� */
void ct_iic_init(void);                      /* ��ʼ��IIC��IO�� */
void ct_iic_start(void);                     /* ����IIC��ʼ�ź� */
void ct_iic_stop(void);                      /* ����IICֹͣ�ź� */
void ct_iic_ack(void);                       /* IIC����ACK�ź� */
void ct_iic_nack(void);                      /* IIC������ACK�ź� */
uint8_t ct_iic_wait_ack(void);               /* IIC�ȴ�ACK�ź� */
void ct_iic_send_byte(uint8_t txd);          /* IIC����һ���ֽ� */
uint8_t ct_iic_read_byte(uint8_t ack);       /* IIC��ȡһ���ֽ� */

#endif







