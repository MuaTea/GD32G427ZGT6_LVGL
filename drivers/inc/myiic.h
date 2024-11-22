#ifndef __MYIIC_H
#define __MYIIC_H

#include <stdint.h>

/* IO�������� */
#define IIC_SCL(x)      do{ x ? \
                            gpio_bit_write(GPIOB, GPIO_PIN_8, SET) : \
                            gpio_bit_write(GPIOB, GPIO_PIN_8, RESET); \
                        }while(0)         /* SCL */ 

#define IIC_SDA(x)      do{ x ? \
                            gpio_bit_write(GPIOB, GPIO_PIN_9, SET) : \
                            gpio_bit_write(GPIOB, GPIO_PIN_9, RESET); \
                        }while(0)         /* SDA */

#define IIC_READ_SDA    gpio_input_bit_get(GPIOB,GPIO_PIN_9)  /* ��ȡSDA */

/* IIC���в������� */
void iic_init(void);                /* ��ʼ��IIC��IO�� */
void iic_start(void);               /* ����IIC��ʼ�ź� */
void iic_stop(void);                /* ����IICֹͣ�ź� */
void iic_ack(void);                 /* IIC����ACK�ź� */
void iic_nack(void);                /* IIC������ACK�ź� */
uint8_t iic_wait_ack(void);         /* IIC�ȴ�ACK�ź� */
void iic_send_byte(uint8_t txd);    /* IIC����һ���ֽ� */
uint8_t iic_read_byte(uint8_t ack); /* IIC��ȡһ���ֽ� */
                        
#endif
















