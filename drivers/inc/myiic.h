#ifndef __MYIIC_H
#define __MYIIC_H

#include <stdint.h>

/* IO操作函数 */
#define IIC_SCL(x)      do{ x ? \
                            gpio_bit_write(GPIOB, GPIO_PIN_8, SET) : \
                            gpio_bit_write(GPIOB, GPIO_PIN_8, RESET); \
                        }while(0)         /* SCL */ 

#define IIC_SDA(x)      do{ x ? \
                            gpio_bit_write(GPIOB, GPIO_PIN_9, SET) : \
                            gpio_bit_write(GPIOB, GPIO_PIN_9, RESET); \
                        }while(0)         /* SDA */

#define IIC_READ_SDA    gpio_input_bit_get(GPIOB,GPIO_PIN_9)  /* 读取SDA */

/* IIC所有操作函数 */
void iic_init(void);                /* 初始化IIC的IO口 */
void iic_start(void);               /* 发送IIC开始信号 */
void iic_stop(void);                /* 发送IIC停止信号 */
void iic_ack(void);                 /* IIC发送ACK信号 */
void iic_nack(void);                /* IIC不发送ACK信号 */
uint8_t iic_wait_ack(void);         /* IIC等待ACK信号 */
void iic_send_byte(uint8_t txd);    /* IIC发送一个字节 */
uint8_t iic_read_byte(uint8_t ack); /* IIC读取一个字节 */
                        
#endif
















