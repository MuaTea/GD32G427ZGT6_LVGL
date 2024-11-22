#ifndef __KEY_DRV_H
#define __KEY_DRV_H

#include <stdint.h>


#define KEY0        gpio_input_bit_get(GPIOE,GPIO_PIN_4)   /* ��ȡKEY0���� */
#define WK_UP       gpio_input_bit_get(GPIOA,GPIO_PIN_0)   /* ��ȡWKUP���� */


#define KEY0_PRES 	1             /* KEY0���� */
#define WKUP_PRES   2             /* KEY_UP����(��WK_UP) */

void key_init(void);              /* ������ʼ������ */
uint8_t key_scan(uint8_t mode);   /* ����ɨ�躯�� */

#endif
