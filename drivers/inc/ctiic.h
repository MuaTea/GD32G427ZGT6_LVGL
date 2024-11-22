#ifndef __CTIIC_H
#define __CTIIC_H
  
#include <stdint.h>

/* IO操作函数 */ 
#define CT_IIC_SCL(x)    do{ x ? \
                             gpio_bit_write(GPIOB, GPIO_PIN_0, SET) : \
                             gpio_bit_write(GPIOB, GPIO_PIN_0, RESET); \
                         }while(0)         /* SCL */ 

#define CT_IIC_SDA(x)    do{ x ? \
                             gpio_bit_write(GPIOF, GPIO_PIN_11, SET) : \
                             gpio_bit_write(GPIOF, GPIO_PIN_11, RESET); \
                         }while(0)         /* SDA */

#define CT_READ_SDA      gpio_input_bit_get(GPIOF, GPIO_PIN_11)  			/* 读取SDA */ 
 

/* IIC所有操作函数 */
void ct_iic_init(void);                      /* 初始化IIC的IO口 */
void ct_iic_start(void);                     /* 发送IIC开始信号 */
void ct_iic_stop(void);                      /* 发送IIC停止信号 */
void ct_iic_ack(void);                       /* IIC发送ACK信号 */
void ct_iic_nack(void);                      /* IIC不发送ACK信号 */
uint8_t ct_iic_wait_ack(void);               /* IIC等待ACK信号 */
void ct_iic_send_byte(uint8_t txd);          /* IIC发送一个字节 */
uint8_t ct_iic_read_byte(uint8_t ack);       /* IIC读取一个字节 */

#endif







