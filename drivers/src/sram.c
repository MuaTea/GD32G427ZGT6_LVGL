#include "sram.h"
#include "usart_drv.h" 


/**
 * @brief       初始化 外部SRAM
 * @param       无
 * @retval      无
 */
void sram_init(void)
{
    exmc_norsram_parameter_struct  nor_init_struct;
    exmc_norsram_timing_parameter_struct  sram_timing_init_struct;
  
    rcu_periph_clock_enable(RCU_EXMC);      /* 使能EXMC时钟 */
  
    /* IO 及 时钟配置 */
    rcu_periph_clock_enable(RCU_GPIOD);     /* 使能GPIOD时钟 */
    rcu_periph_clock_enable(RCU_GPIOE);     /* 使能GPIOE时钟 */
    rcu_periph_clock_enable(RCU_GPIOF);     /* 使能GPIOF时钟 */
    rcu_periph_clock_enable(RCU_GPIOG);     /* 使能GPIOG时钟 */       
        
    /* 设置PD0,1,4,5,8~15  复用推挽输出 */
	  gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 |
                GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 |
                  GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 |GPIO_PIN_9 |
                            GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    /* 设置PE0,1,7~15  复用推挽输出 */
	  gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                  GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 |GPIO_PIN_9 | GPIO_PIN_10 |
                            GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    /* 设置PF0~5,12~15  复用推挽输出 */
    gpio_af_set(GPIOF, GPIO_AF_12, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
                GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
                  GPIO_PIN_12 |GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |GPIO_PIN_4 | GPIO_PIN_5 |
                            GPIO_PIN_12 |GPIO_PIN_13 |GPIO_PIN_14 | GPIO_PIN_15);
	
    /* 设置PG0~5,10  复用推挽输出 */
    gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10);
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10);
     
    /* 配置时序参数 */
    sram_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A;            /* 异步访问模式A */
    sram_timing_init_struct.syn_data_latency = EXMC_DATALAT_2_CLK;            /* NOR Flash数据延时，此处未用到 */
    sram_timing_init_struct.syn_clk_division = EXMC_SYN_CLOCK_RATIO_DISABLE;  /* 同步模式时钟分频比，此处未用到  */
    sram_timing_init_struct.bus_latency = 0;                                  /* 总线延迟时间，此处未用到 */
    sram_timing_init_struct.asyn_data_setuptime = 8;                          /* 数据保存时间(DET)为8个HCLK 1/200M = 5ns * 8 = 40ns */
    sram_timing_init_struct.asyn_address_holdtime = 0;                        /* 地址保持时间(AHLD)模式A未用到 */
    sram_timing_init_struct.asyn_address_setuptime = 2;                       /* 地址建立时间(ASET)为2个HCLK = 5ns * 2 = 10ns */

    /* 配置EXMC参数 */
    nor_init_struct.norsram_region = EXMC_BANK0_NORSRAM_REGION2;       /* 选择EXMC_BANK0_NORSRAM_REGION2 */   
    nor_init_struct.write_mode = EXMC_ASYN_WRITE;                      /* 选择写操作模式(同步模式或者异步模式)，此处未用到 */
    nor_init_struct.extended_mode = DISABLE;                           /* 不使用扩展模式 */
    nor_init_struct.asyn_wait = DISABLE;                               /* 禁用异步等待功能 */
    nor_init_struct.nwait_signal = DISABLE;                            /* 在同步突发模式中，使能或者禁用NWAIT信号，此处未用到 */
    nor_init_struct.memory_write = ENABLE;                             /* 使能写操作 */
    nor_init_struct.nwait_config = EXMC_NWAIT_CONFIG_BEFORE;           /* NWAIT信号配置，只在同步模式有效，此处未用到 */
    nor_init_struct.wrap_burst_mode = DISABLE;                         /* 禁用非对齐成组模式 */
    nor_init_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;          /* NWAIT的极性，此处未用到 */
    nor_init_struct.burst_mode = DISABLE;                              /* 禁用突发模式 */
    nor_init_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_16B;        /* 存储器数据总线宽度16位 */
    nor_init_struct.memory_type = EXMC_MEMORY_TYPE_SRAM;               /* 外部存储器的类型为SRAM */
    nor_init_struct.address_data_mux = DISABLE;                        /* 数据线/地址线不复用 */
    nor_init_struct.read_write_timing = &sram_timing_init_struct;      /* 读时序参数和写时序参数 */
    nor_init_struct.write_timing = &sram_timing_init_struct;           /* 未用扩展模式，没有用到 */
	
    exmc_norsram_init(&nor_init_struct);                               /* 初始化EXMC NOR/SRAM bank0 region2 */
	
    exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION2);                   /* 使能EXMC NOR/SRAM bank0 region2 */ 
}

/**
 * @brief       往SRAM指定地址写入指定长度数据
 * @param       pbuf    : 数据存储区
 * @param       addr    : 开始写入的地址
 * @param       datalen : 要写入的字节数
 * @retval      无
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
 * @brief       从SRAM指定地址读取指定长度数据
 * @param       pbuf    : 数据存储区
 * @param       addr    : 开始读取的地址
 * @param       datalen : 要读取的字节数
 * @retval      无
 */
void sram_read(uint8_t *pbuf, uint32_t addr, uint32_t datalen)
{
    for (; datalen != 0; datalen--)
    {
        *pbuf++ = *(volatile uint8_t *)(SRAM_BASE_ADDR + addr);
        addr++;
    }
}

/*******************测试函数**********************************/

/**
 * @brief       测试函数 在SRAM指定地址写入1个字节
 * @param       addr    : 开始写入的地址(0~SRAM大小)
 * @param       data    : 要写入的字节
 * @retval      无
 */
void sram_test_write(uint32_t addr, uint8_t data)
{
    sram_write(&data, addr, 1); /* 写入1个字节 */
}

/**
 * @brief       测试函数 在SRAM指定地址读取1个字节
 * @param       addr    : 开始读取的地址(0~SRAM大小)
 * @retval      读取到的数据(1个字节)
 */
uint8_t sram_test_read(uint32_t addr)
{
    uint8_t data;
    sram_read(&data, addr, 1); /* 读取1个字节 */
    return data;
}
