#include "string.h"
#include "lcd.h"
#include "touch.h"
#include "ctiic.h"
#include "gt1151q.h"
#include "usart_drv.h"
#include "systick.h"


/**
 * @brief       向gt1151写入一次数据
 * @param       reg : 起始寄存器地址
 * @param       buf : 数据缓存区
 * @param       len : 写数据长度
 * @retval      0, 成功; 1, 失败;
 */
uint8_t gt1151_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
		uint8_t i;
		uint8_t ret=0;
  
    ct_iic_start();
    ct_iic_send_byte(GT_CMD_WR);        /* 发送写命令 */
    ct_iic_wait_ack();
    ct_iic_send_byte(reg >> 8);         /* 发送高8位地址 */
    ct_iic_wait_ack();
    ct_iic_send_byte(reg & 0XFF);       /* 发送低8位地址 */
    ct_iic_wait_ack();
  
    for (i = 0; i < len; i++)
    {
        ct_iic_send_byte(buf[i]);       /* 发数据 */
        ret = ct_iic_wait_ack();

        if (ret) break;
    }

    ct_iic_stop();  /* 产生一个停止条件 */
    return ret;
}

/**
 * @brief       从gt1151读出一次数据
 * @param       reg : 起始寄存器地址
 * @param       buf : 数据缓存区
 * @param       len : 读数据长度
 * @retval      无
 */	  
void gt1151_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;

    ct_iic_start();
    ct_iic_send_byte(GT_CMD_WR);        /* 发送写命令 */
    ct_iic_wait_ack();
    ct_iic_send_byte(reg >> 8);         /* 发送高8位地址 */
    ct_iic_wait_ack();
    ct_iic_send_byte(reg & 0XFF);       /* 发送低8位地址 */
    ct_iic_wait_ack();
    ct_iic_start();
    ct_iic_send_byte(GT_CMD_RD);        /* 发送读命令 */
    ct_iic_wait_ack();
  
    for (i = 0; i < len; i++)
    {
        buf[i] = ct_iic_read_byte(i == (len - 1) ? 0 : 1);  /* 读取数据 */
    }

    ct_iic_stop();  /* 产生一个停止条件 */
} 

/**
 * @brief       初始化GT1151触摸屏
 * @param       无
 * @retval      0, 初始化成功; 1, 初始化失败;
 */
uint8_t gt1151_init(void)
{
		uint8_t temp[5];  

    rcu_periph_clock_enable(RCU_GPIOB);     /* 使能GPIOB时钟 */
    rcu_periph_clock_enable(RCU_GPIOC);     /* 使能GPIOC时钟 */
  
    /* 设置GT_RST引脚 推挽输出模式 */
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_13);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
  
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_1);  /* GT_INT引脚模式设置,上拉输入 */
	
    ct_iic_init();      /* 初始化电容屏的I2C总线 */
    GT_RST(0);          /* 复位 */
    delay_ms(50);
    GT_RST(1);          /* 释放复位 */
    delay_ms(50);
	
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_1);    /* GT_INT引脚模式设置,浮空输入 */
		
    delay_ms(100);
    gt1151_rd_reg(GT_PID_REG, temp, 4);  /* 读取触摸IC的ID */
    temp[4] = 0;
    
		printf("CTP ID:%s\r\n", temp);	     /* 打印ID */
		if (strcmp((char*)temp,"1158") == 0) /* ID==1158 */
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/* GT1151 5个触摸点 对应的寄存器表 */
const uint16_t GT1151_TPX_TBL[5] = {GT_TP1_REG, GT_TP2_REG, GT_TP3_REG, GT_TP4_REG, GT_TP5_REG};

/**
 * @brief       扫描触摸屏(采用查询方式)
 * @param       mode : 0，正常扫描，电容屏未用到此参数, 为了兼容电阻屏
 * @retval      当前触屏状态
 *   @arg       0, 触屏无触摸; 
 *   @arg       1, 触屏有触摸;
 */
uint8_t gt1151_scan(uint8_t mode)
{
    uint8_t buf[4];
    uint8_t i = 0;
    uint8_t res = 0;
    uint8_t temp;
    uint8_t tempsta;
		static uint8_t t = 0;   /* 控制查询间隔,从而降低CPU占用率 */   
		t++;
  
		if ((t % 10) == 0 || t < 10)    /* 空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率 */
		{
        gt1151_rd_reg(GT_GSTID_REG, &mode, 1);  /* 读取触摸点的状态 */
      
        if ((mode & 0X80) && ((mode & 0XF) <= 5))
        {
            i = 0;
            gt1151_wr_reg(GT_GSTID_REG, &i, 1); /* 清标志 */
        }
      
			  if ((mode & 0XF) && ((mode & 0XF) <= 5))
        {
            temp = 0XFF << (mode & 0XF);            /* 将点的个数转换为1的位数,匹配tp_dev.sta定义 */
            tempsta = tp_dev.sta;                   /* 保存当前的tp_dev.sta值 */
            tp_dev.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;
            tp_dev.x[4] = tp_dev.x[0];              /* 保存触点0的数据,保存在最后一个上 */
            tp_dev.y[4] = tp_dev.y[0];        
          
            for (i = 0; i < 5; i++)
            {
                if (tp_dev.sta & (1 << i))  /* 触摸有效? */
                {
                    gt1151_rd_reg(GT1151_TPX_TBL[i], buf, 4);   /* 读取XY坐标值 */

                    if(tp_dev.touchtype & 0X01)  /* 横屏 */
                    {
                        tp_dev.x[i] = lcddev.width - (((uint16_t)buf[3] << 8) + buf[2]);
                        tp_dev.y[i] = ((uint16_t)buf[1] << 8) + buf[0];                      
                    }
                    else
                    {
                        tp_dev.x[i] = ((uint16_t)buf[1] << 8) + buf[0];
                        tp_dev.y[i] = ((uint16_t)buf[3] << 8) + buf[2];                    
                    }   

                    //printf("x[%d]:%d,y[%d]:%d\r\n", i, tp_dev.x[i], i, tp_dev.y[i]);  
                }			
            } 
            
            res = 1;

            if (tp_dev.x[0] > lcddev.width || tp_dev.y[0] > lcddev.height)  /* 非法数据(坐标超出了) */
            {
                if ((mode & 0XF) > 1)   /* 有其他点有数据,则复制第二个触点的数据到第一个触点. */
                {
                    tp_dev.x[0] = tp_dev.x[1];
                    tp_dev.y[0] = tp_dev.y[1];
                    t = 0;  /* 触发一次,则会最少连续监测10次,从而提高命中率 */
                }
                else        /* 非法数据,则忽略此次数据(还原原来的) */
                {
                    tp_dev.x[0] = tp_dev.x[4];
                    tp_dev.y[0] = tp_dev.y[4];
                    mode = 0X80;
                    tp_dev.sta = tempsta;   /* 恢复tp_dev.sta */
                }
            }
            else 
            {
                t = 0;       /* 触发一次,则会最少连续监测10次,从而提高命中率 */
            }            
        }
		}
    
    if ((mode & 0X8F) == 0X80)  /* 无触摸点按下 */
    {
        if (tp_dev.sta & TP_PRES_DOWN)      /* 之前是被按下的 */
        {
            tp_dev.sta &= ~TP_PRES_DOWN;    /* 标记按键松开 */
        }
        else    /* 之前就没有被按下 */
        {
            tp_dev.x[0] = 0xffff;
            tp_dev.y[0] = 0xffff;
            tp_dev.sta &= 0XE0;             /* 清除点有效标记 */
        }
    }
    
    if (t > 240) t = 10; /* 重新从10开始计数 */

    return res;
}
 



























