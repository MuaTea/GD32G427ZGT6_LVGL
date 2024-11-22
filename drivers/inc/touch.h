#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "gt1151q.h"
#include "ft5206.h"


#define TP_PRES_DOWN    0x80  		/* ������������ */	  
#define TP_CATH_PRES    0x40  	 	/* �а��������� */ 
#define CT_MAX_TOUCH    5    		  /* ������֧�ֵĵ���,�̶�Ϊ5�� */

/* ������������ */
typedef struct 
{
    uint8_t (*init)(void);      /* ��ʼ�������������� */
    uint8_t (*scan)(uint8_t);   /* ɨ�败����.0,��Ļɨ��;1,��������; */
    void (*adjust)(void);       /* ������У׼ */
    uint16_t x[CT_MAX_TOUCH];   /* ��ǰ���� */
    uint16_t y[CT_MAX_TOUCH];   /* �����������5������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
                                 * x[4],y[4]�洢��һ�ΰ���ʱ������.
                                 */

    uint8_t sta;                /* �ʵ�״̬
                                 * b7:����1/�ɿ�0;
                                 * b6:0,û�а�������;1,�а�������.
                                 * b5:����
                                 * b4~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
                                 */

    /* 5��У׼������У׼����(����������ҪУ׼) */
    float xfac;                 /* 5��У׼��x����������� */
    float yfac;                 /* 5��У׼��y����������� */
    short xc;                   /* ����X��������ֵ(ADֵ) */
    short yc;                   /* ����Y��������ֵ(ADֵ) */

    /* ��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
     * b0:0, ����(�ʺ�����ΪX����,����ΪY�����TP)
     *    1, ����(�ʺ�����ΪY����,����ΪX�����TP)
     * b1~6: ����.
     * b7:0, ������
     *    1, ������
     */
    uint8_t touchtype;    
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	/* ������������touch.c���涨�� */

/* ���败������������ */
#define T_PEN           gpio_input_bit_get(GPIOB,GPIO_PIN_1)         /* T_PEN */  
#define T_MISO          gpio_input_bit_get(GPIOB,GPIO_PIN_2)         /* T_MISO */  

#define T_MOSI(x)       do{ x ? \
                            gpio_bit_write(GPIOF, GPIO_PIN_11, SET) : \
                            gpio_bit_write(GPIOF, GPIO_PIN_11, RESET); \
                        }while(0)            /* T_MOSI */ 

#define T_CLK(x)        do{ x ? \
                            gpio_bit_write(GPIOB, GPIO_PIN_0, SET) : \
                            gpio_bit_write(GPIOB, GPIO_PIN_0, RESET); \
                        }while(0)            /* T_CLK */ 

#define T_CS(x)         do{ x ? \
                            gpio_bit_write(GPIOC, GPIO_PIN_13, SET) : \
                            gpio_bit_write(GPIOC, GPIO_PIN_13, RESET); \
                        }while(0)            /* T_CS */ 

   
/* ���������� */
static void tp_write_byte(uint8_t data);                /* �����оƬд��һ������ */
static uint16_t tp_read_ad(uint8_t cmd);                /* ��ȡADת��ֵ */
static uint16_t tp_read_xoy(uint8_t cmd);               /* ���˲��������ȡ(X/Y) */
static void tp_read_xy(uint16_t *x, uint16_t *y);       /* ˫�����ȡ(X+Y) */
static uint8_t tp_read_xy2(uint16_t *x, uint16_t *y);   /* ����ǿ�˲���˫���������ȡ */
static void tp_draw_touch_point(uint16_t x, uint16_t y, uint16_t color);    /* ��һ������У׼�� */
static void tp_adjust_info_show(uint16_t xy[5][2], double px, double py);   /* ��ʾУ׼��Ϣ */

void tp_draw_big_point(uint16_t x, uint16_t y, uint16_t color);	/* ��һ����� */
void tp_save_adjust_data(void);				/* ����У׼���� */
uint8_t tp_get_adjust_data(void);		  /* ��ȡУ׼���� */
void tp_adjust(void);							    /* ������У׼ */
uint8_t rtp_init(void);               /* ���败������ʼ�� */
                        
uint8_t tp_scan(uint8_t mode);				/* ����ɨ�� */
uint8_t tp_init(void);							  /* ��������ʼ�� */

#endif

















