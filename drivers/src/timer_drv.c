#include "timer_drv.h"
#include "lvgl.h"

/**
 * @brief       ������ʱ��TIMER6��ʱ�жϳ�ʼ��
 * @note
 *              ������ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ������ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ50M, ���Զ�ʱ��TIMER6ʱ�� = 100Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void timer6_init(uint16_t arr, uint16_t psc)
{
    timer_parameter_struct timer_initpara;               /* timer_initpara���ڴ�Ŷ�ʱ���Ĳ��� */

    /* ʹ��RCU���ʱ�� */ 
    rcu_periph_clock_enable(RCU_TIMER6);                 /* ʹ��TIMER6��ʱ�� */

    /* ��λTIMER6 */
    timer_deinit(TIMER6);                                /* ��λTIMER6 */
    timer_struct_para_init(&timer_initpara);             /* ��ʼ��timer_initparaΪĬ��ֵ */

    /* ����TIMER6 */
    timer_initpara.prescaler         = psc;              /* ����Ԥ��Ƶֵ */
    timer_initpara.counterdirection  = TIMER_COUNTER_UP; /* �������ϼ���ģʽ */
    timer_initpara.period            = arr;              /* �����Զ���װ��ֵ */
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1; /* ����ʱ�ӷ�Ƶ���� */
    timer_init(TIMER6, &timer_initpara);                 /* ���ݲ�����ʼ����ʱ�� */

    /* ʹ�ܶ�ʱ�������ж� */
    timer_interrupt_enable(TIMER6, TIMER_INT_UP);        /* ʹ�ܶ�ʱ���ĸ����ж� */
    nvic_irq_enable(TIMER6_IRQn, 1, 3);                  /* ����NVIC�������ȼ�����ռ���ȼ�1����Ӧ���ȼ�3 */
    timer_enable(TIMER6);                                /* ʹ�ܶ�ʱ��TIMER6 */
}

/**
 * @brief       ������ʱ��TIMER6�жϷ�����
 * @param       ��
 * @retval      ��
 */
void TIMER6_IRQHandler(void)
{
    if(timer_interrupt_flag_get(TIMER6, TIMER_INT_FLAG_UP) == SET)     /* �ж϶�ʱ�������ж��Ƿ��� */
    {
		lv_tick_inc(1);
        timer_interrupt_flag_clear(TIMER6, TIMER_INT_FLAG_UP);         /* �����ʱ�������жϱ�־ */
    }
}














