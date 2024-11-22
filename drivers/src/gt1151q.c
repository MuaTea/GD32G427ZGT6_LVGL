#include "string.h"
#include "lcd.h"
#include "touch.h"
#include "ctiic.h"
#include "gt1151q.h"
#include "usart_drv.h"
#include "systick.h"


/**
 * @brief       ��gt1151д��һ������
 * @param       reg : ��ʼ�Ĵ�����ַ
 * @param       buf : ���ݻ�����
 * @param       len : д���ݳ���
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t gt1151_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
		uint8_t i;
		uint8_t ret=0;
  
    ct_iic_start();
    ct_iic_send_byte(GT_CMD_WR);        /* ����д���� */
    ct_iic_wait_ack();
    ct_iic_send_byte(reg >> 8);         /* ���͸�8λ��ַ */
    ct_iic_wait_ack();
    ct_iic_send_byte(reg & 0XFF);       /* ���͵�8λ��ַ */
    ct_iic_wait_ack();
  
    for (i = 0; i < len; i++)
    {
        ct_iic_send_byte(buf[i]);       /* ������ */
        ret = ct_iic_wait_ack();

        if (ret) break;
    }

    ct_iic_stop();  /* ����һ��ֹͣ���� */
    return ret;
}

/**
 * @brief       ��gt1151����һ������
 * @param       reg : ��ʼ�Ĵ�����ַ
 * @param       buf : ���ݻ�����
 * @param       len : �����ݳ���
 * @retval      ��
 */	  
void gt1151_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;

    ct_iic_start();
    ct_iic_send_byte(GT_CMD_WR);        /* ����д���� */
    ct_iic_wait_ack();
    ct_iic_send_byte(reg >> 8);         /* ���͸�8λ��ַ */
    ct_iic_wait_ack();
    ct_iic_send_byte(reg & 0XFF);       /* ���͵�8λ��ַ */
    ct_iic_wait_ack();
    ct_iic_start();
    ct_iic_send_byte(GT_CMD_RD);        /* ���Ͷ����� */
    ct_iic_wait_ack();
  
    for (i = 0; i < len; i++)
    {
        buf[i] = ct_iic_read_byte(i == (len - 1) ? 0 : 1);  /* ��ȡ���� */
    }

    ct_iic_stop();  /* ����һ��ֹͣ���� */
} 

/**
 * @brief       ��ʼ��GT1151������
 * @param       ��
 * @retval      0, ��ʼ���ɹ�; 1, ��ʼ��ʧ��;
 */
uint8_t gt1151_init(void)
{
		uint8_t temp[5];  

    rcu_periph_clock_enable(RCU_GPIOB);     /* ʹ��GPIOBʱ�� */
    rcu_periph_clock_enable(RCU_GPIOC);     /* ʹ��GPIOCʱ�� */
  
    /* ����GT_RST���� �������ģʽ */
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_13);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
  
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_1);  /* GT_INT����ģʽ����,�������� */
	
    ct_iic_init();      /* ��ʼ����������I2C���� */
    GT_RST(0);          /* ��λ */
    delay_ms(50);
    GT_RST(1);          /* �ͷŸ�λ */
    delay_ms(50);
	
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_1);    /* GT_INT����ģʽ����,�������� */
		
    delay_ms(100);
    gt1151_rd_reg(GT_PID_REG, temp, 4);  /* ��ȡ����IC��ID */
    temp[4] = 0;
    
		printf("CTP ID:%s\r\n", temp);	     /* ��ӡID */
		if (strcmp((char*)temp,"1158") == 0) /* ID==1158 */
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/* GT1151 5�������� ��Ӧ�ļĴ����� */
const uint16_t GT1151_TPX_TBL[5] = {GT_TP1_REG, GT_TP2_REG, GT_TP3_REG, GT_TP4_REG, GT_TP5_REG};

/**
 * @brief       ɨ�败����(���ò�ѯ��ʽ)
 * @param       mode : 0������ɨ�裬������δ�õ��˲���, Ϊ�˼��ݵ�����
 * @retval      ��ǰ����״̬
 *   @arg       0, �����޴���; 
 *   @arg       1, �����д���;
 */
uint8_t gt1151_scan(uint8_t mode)
{
    uint8_t buf[4];
    uint8_t i = 0;
    uint8_t res = 0;
    uint8_t temp;
    uint8_t tempsta;
		static uint8_t t = 0;   /* ���Ʋ�ѯ���,�Ӷ�����CPUռ���� */   
		t++;
  
		if ((t % 10) == 0 || t < 10)    /* ����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ���� */
		{
        gt1151_rd_reg(GT_GSTID_REG, &mode, 1);  /* ��ȡ�������״̬ */
      
        if ((mode & 0X80) && ((mode & 0XF) <= 5))
        {
            i = 0;
            gt1151_wr_reg(GT_GSTID_REG, &i, 1); /* ���־ */
        }
      
			  if ((mode & 0XF) && ((mode & 0XF) <= 5))
        {
            temp = 0XFF << (mode & 0XF);            /* ����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� */
            tempsta = tp_dev.sta;                   /* ���浱ǰ��tp_dev.staֵ */
            tp_dev.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;
            tp_dev.x[4] = tp_dev.x[0];              /* ���津��0������,���������һ���� */
            tp_dev.y[4] = tp_dev.y[0];        
          
            for (i = 0; i < 5; i++)
            {
                if (tp_dev.sta & (1 << i))  /* ������Ч? */
                {
                    gt1151_rd_reg(GT1151_TPX_TBL[i], buf, 4);   /* ��ȡXY����ֵ */

                    if(tp_dev.touchtype & 0X01)  /* ���� */
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

            if (tp_dev.x[0] > lcddev.width || tp_dev.y[0] > lcddev.height)  /* �Ƿ�����(���곬����) */
            {
                if ((mode & 0XF) > 1)   /* ��������������,���Ƶڶ�����������ݵ���һ������. */
                {
                    tp_dev.x[0] = tp_dev.x[1];
                    tp_dev.y[0] = tp_dev.y[1];
                    t = 0;  /* ����һ��,��������������10��,�Ӷ���������� */
                }
                else        /* �Ƿ�����,����Դ˴�����(��ԭԭ����) */
                {
                    tp_dev.x[0] = tp_dev.x[4];
                    tp_dev.y[0] = tp_dev.y[4];
                    mode = 0X80;
                    tp_dev.sta = tempsta;   /* �ָ�tp_dev.sta */
                }
            }
            else 
            {
                t = 0;       /* ����һ��,��������������10��,�Ӷ���������� */
            }            
        }
		}
    
    if ((mode & 0X8F) == 0X80)  /* �޴����㰴�� */
    {
        if (tp_dev.sta & TP_PRES_DOWN)      /* ֮ǰ�Ǳ����µ� */
        {
            tp_dev.sta &= ~TP_PRES_DOWN;    /* ��ǰ����ɿ� */
        }
        else    /* ֮ǰ��û�б����� */
        {
            tp_dev.x[0] = 0xffff;
            tp_dev.y[0] = 0xffff;
            tp_dev.sta &= 0XE0;             /* �������Ч��� */
        }
    }
    
    if (t > 240) t = 10; /* ���´�10��ʼ���� */

    return res;
}
 



























