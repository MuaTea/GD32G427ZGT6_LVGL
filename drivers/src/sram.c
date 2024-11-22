#include "sram.h"
#include "usart_drv.h" 


/**
 * @brief       ��ʼ�� �ⲿSRAM
 * @param       ��
 * @retval      ��
 */
void sram_init(void)
{
    exmc_norsram_parameter_struct  nor_init_struct;
    exmc_norsram_timing_parameter_struct  sram_timing_init_struct;
  
    rcu_periph_clock_enable(RCU_EXMC);      /* ʹ��EXMCʱ�� */
  
    /* IO �� ʱ������ */
    rcu_periph_clock_enable(RCU_GPIOD);     /* ʹ��GPIODʱ�� */
    rcu_periph_clock_enable(RCU_GPIOE);     /* ʹ��GPIOEʱ�� */
    rcu_periph_clock_enable(RCU_GPIOF);     /* ʹ��GPIOFʱ�� */
    rcu_periph_clock_enable(RCU_GPIOG);     /* ʹ��GPIOGʱ�� */       
        
    /* ����PD0,1,4,5,8~15  ����������� */
	  gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 |
                GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 |
                  GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 |GPIO_PIN_9 |
                            GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    /* ����PE0,1,7~15  ����������� */
	  gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                  GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 |GPIO_PIN_9 | GPIO_PIN_10 |
                            GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    /* ����PF0~5,12~15  ����������� */
    gpio_af_set(GPIOF, GPIO_AF_12, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
                GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
                  GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |GPIO_PIN_4 | GPIO_PIN_5 |
                            GPIO_PIN_12 |GPIO_PIN_13 |GPIO_PIN_14 | GPIO_PIN_15);
	
    /* ����PG0~5,10  ����������� */
    gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10);
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10);
     
    /* ����ʱ����� */
    sram_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A;            /* �첽����ģʽA */
    sram_timing_init_struct.syn_data_latency = EXMC_DATALAT_2_CLK;            /* NOR Flash������ʱ���˴�δ�õ� */
    sram_timing_init_struct.syn_clk_division = EXMC_SYN_CLOCK_RATIO_DISABLE;  /* ͬ��ģʽʱ�ӷ�Ƶ�ȣ��˴�δ�õ�  */
    sram_timing_init_struct.bus_latency = 0;                                  /* �����ӳ�ʱ�䣬�˴�δ�õ� */
    sram_timing_init_struct.asyn_data_setuptime = 8;                          /* ���ݱ���ʱ��(DET)Ϊ8��HCLK 1/200M = 5ns * 8 = 40ns */
    sram_timing_init_struct.asyn_address_holdtime = 0;                        /* ��ַ����ʱ��(AHLD)ģʽAδ�õ� */
    sram_timing_init_struct.asyn_address_setuptime = 2;                       /* ��ַ����ʱ��(ASET)Ϊ2��HCLK = 5ns * 2 = 10ns */

    /* ����EXMC���� */
    nor_init_struct.norsram_region = EXMC_BANK0_NORSRAM_REGION2;       /* ѡ��EXMC_BANK0_NORSRAM_REGION2 */   
    nor_init_struct.write_mode = EXMC_ASYN_WRITE;                      /* ѡ��д����ģʽ(ͬ��ģʽ�����첽ģʽ)���˴�δ�õ� */
    nor_init_struct.extended_mode = DISABLE;                           /* ��ʹ����չģʽ */
    nor_init_struct.asyn_wait = DISABLE;                               /* �����첽�ȴ����� */
    nor_init_struct.nwait_signal = DISABLE;                            /* ��ͬ��ͻ��ģʽ�У�ʹ�ܻ��߽���NWAIT�źţ��˴�δ�õ� */
    nor_init_struct.memory_write = ENABLE;                             /* ʹ��д���� */
    nor_init_struct.nwait_config = EXMC_NWAIT_CONFIG_BEFORE;           /* NWAIT�ź����ã�ֻ��ͬ��ģʽ��Ч���˴�δ�õ� */
    nor_init_struct.wrap_burst_mode = DISABLE;                         /* ���÷Ƕ������ģʽ */
    nor_init_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;          /* NWAIT�ļ��ԣ��˴�δ�õ� */
    nor_init_struct.burst_mode = DISABLE;                              /* ����ͻ��ģʽ */
    nor_init_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_16B;        /* �洢���������߿��16λ */
    nor_init_struct.memory_type = EXMC_MEMORY_TYPE_SRAM;               /* �ⲿ�洢��������ΪSRAM */
    nor_init_struct.address_data_mux = DISABLE;                        /* ������/��ַ�߲����� */
    nor_init_struct.read_write_timing = &sram_timing_init_struct;      /* ��ʱ�������дʱ����� */
    nor_init_struct.write_timing = &sram_timing_init_struct;           /* δ����չģʽ��û���õ� */
	
    exmc_norsram_init(&nor_init_struct);                               /* ��ʼ��EXMC NOR/SRAM bank0 region2 */
	
    exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION2);                   /* ʹ��EXMC NOR/SRAM bank0 region2 */ 
}

/**
 * @brief       ��SRAMָ����ַд��ָ����������
 * @param       pbuf    : ���ݴ洢��
 * @param       addr    : ��ʼд��ĵ�ַ
 * @param       datalen : Ҫд����ֽ���
 * @retval      ��
 */
void sram_write(uint8_t *pbuf, uint32_t addr, uint32_t datalen)
{
    for (; datalen != 0; datalen--)
    {
        *(volatile uint8_t *)(SRAM_BASE_ADDR + addr) = *pbuf;
        addr++;
        pbuf++;
    }
}

/**
 * @brief       ��SRAMָ����ַ��ȡָ����������
 * @param       pbuf    : ���ݴ洢��
 * @param       addr    : ��ʼ��ȡ�ĵ�ַ
 * @param       datalen : Ҫ��ȡ���ֽ���
 * @retval      ��
 */
void sram_read(uint8_t *pbuf, uint32_t addr, uint32_t datalen)
{
    for (; datalen != 0; datalen--)
    {
        *pbuf++ = *(volatile uint8_t *)(SRAM_BASE_ADDR + addr);
        addr++;
    }
}

/*******************���Ժ���**********************************/

/**
 * @brief       ���Ժ��� ��SRAMָ����ַд��1���ֽ�
 * @param       addr    : ��ʼд��ĵ�ַ(0~SRAM��С)
 * @param       data    : Ҫд����ֽ�
 * @retval      ��
 */
void sram_test_write(uint32_t addr, uint8_t data)
{
    sram_write(&data, addr, 1); /* д��1���ֽ� */
}

/**
 * @brief       ���Ժ��� ��SRAMָ����ַ��ȡ1���ֽ�
 * @param       addr    : ��ʼ��ȡ�ĵ�ַ(0~SRAM��С)
 * @retval      ��ȡ��������(1���ֽ�)
 */
uint8_t sram_test_read(uint32_t addr)
{
    uint8_t data;
    sram_read(&data, addr, 1); /* ��ȡ1���ֽ� */
    return data;
}
