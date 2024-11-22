#include "led_drv.h"

void LED_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOF);     /* 使能GPIOF时钟 */
    
    /* 设置PF9推挽输出模式 */
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);
    /* 设置PF10推挽输出模式 */
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_10);
	
	LED0(1);                                /* 关闭LED0 */ 
	LED1(1);                                /* 关闭LED1 */
}
