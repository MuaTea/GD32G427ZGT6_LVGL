#ifndef __KEY_DRV_H
#define __KEY_DRV_H

#include <stdint.h>


#define KEY0        gpio_input_bit_get(GPIOE,GPIO_PIN_4)   /* 读取KEY0引脚 */
#define WK_UP       gpio_input_bit_get(GPIOA,GPIO_PIN_0)   /* 读取WKUP引脚 */


#define KEY0_PRES 	1             /* KEY0按下 */
#define WKUP_PRES   2             /* KEY_UP按下(即WK_UP) */

void key_init(void);              /* 按键初始化函数 */
uint8_t key_scan(uint8_t mode);   /* 按键扫描函数 */

#endif
