#include "key_drv.h"
#include "systick.h"


/**
 * @brief       ������ʼ������
 * @param       ��
 * @retval      ��
 */
void key_init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);     /* ʹ��GPIOAʱ�� */
    rcu_periph_clock_enable(RCU_GPIOE);     /* ʹ��GPIOEʱ�� */
    
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_0);  /* WK_UP����ģʽ����,�������� */
    
    gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_4);  /* KEY0����ģʽ����,�������� */  	
}

/**
 * @brief       ����ɨ�躯��
 * @note        �ú�������Ӧ���ȼ�(ͬʱ���¶������): WK_UP > KEY0!!
 * @param       mode:0 / 1, ���庬������:
 *   @arg       0,  ��֧��������(���������²���ʱ, ֻ�е�һ�ε��û᷵�ؼ�ֵ,
 *                  �����ɿ��Ժ�, �ٴΰ��²Ż᷵��������ֵ)
 *   @arg       1,  ֧��������(���������²���ʱ, ÿ�ε��øú������᷵�ؼ�ֵ)
 * @retval      ��ֵ, ��������:
 *              KEY0_PRES, 1, KEY0����
 *              WKUP_PRES, 2, WKUP����
 */
uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up = 1;  /* �������ɿ���־ */
    uint8_t keyval = 0;

    if (mode) key_up = 1;       /* ֧������ */
    if (key_up && (KEY0 == 1 || WK_UP == 1))     /* �����ɿ���־Ϊ1, ��������һ������������ */
    {
        delay_ms(10);           /* ȥ���� */
        key_up = 0;

        if (KEY0 == 1)  keyval = KEY0_PRES;
        if (WK_UP == 1) keyval = WKUP_PRES;
    }
    else if (KEY0 == 0 && WK_UP == 0)            /* û���κΰ�������, ��ǰ����ɿ� */
    {
        key_up = 1;
    }

    return keyval;              /* ���ؼ�ֵ */
}


