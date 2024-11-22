#include "led_drv.h"

void LED_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOF);     /* ʹ��GPIOFʱ�� */
    
    /* ����PF9�������ģʽ */
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);
    /* ����PF10�������ģʽ */
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_10);
	
	LED0(1);                                /* �ر�LED0 */ 
	LED1(1);                                /* �ر�LED1 */
}
