#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "gt1151q.h"
#include "ft5206.h"


#define TP_PRES_DOWN    0x80  		/* 触摸屏被按下 */	  
#define TP_CATH_PRES    0x40  	 	/* 有按键按下了 */ 
#define CT_MAX_TOUCH    5    		  /* 电容屏支持的点数,固定为5点 */

/* 触摸屏控制器 */
typedef struct 
{
    uint8_t (*init)(void);      /* 初始化触摸屏控制器 */
    uint8_t (*scan)(uint8_t);   /* 扫描触摸屏.0,屏幕扫描;1,物理坐标; */
    void (*adjust)(void);       /* 触摸屏校准 */
    uint16_t x[CT_MAX_TOUCH];   /* 当前坐标 */
    uint16_t y[CT_MAX_TOUCH];   /* 电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
                                 * x[4],y[4]存储第一次按下时的坐标.
                                 */

    uint8_t sta;                /* 笔的状态
                                 * b7:按下1/松开0;
                                 * b6:0,没有按键按下;1,有按键按下.
                                 * b5:保留
                                 * b4~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
                                 */

    /* 5点校准触摸屏校准参数(电容屏不需要校准) */
    float xfac;                 /* 5点校准法x方向比例因子 */
    float yfac;                 /* 5点校准法y方向比例因子 */
    short xc;                   /* 中心X坐标物理值(AD值) */
    short yc;                   /* 中心Y坐标物理值(AD值) */

    /* 当触摸屏的左右上下完全颠倒时需要用到.
     * b0:0, 竖屏(适合左右为X坐标,上下为Y坐标的TP)
     *    1, 横屏(适合左右为Y坐标,上下为X坐标的TP)
     * b1~6: 保留.
     * b7:0, 电阻屏
     *    1, 电容屏
     */
    uint8_t touchtype;    
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	/* 触屏控制器在touch.c里面定义 */

/* 电阻触摸屏控制引脚 */
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

   
/* 电阻屏函数 */
static void tp_write_byte(uint8_t data);                /* 向控制芯片写入一个数据 */
static uint16_t tp_read_ad(uint8_t cmd);                /* 读取AD转换值 */
static uint16_t tp_read_xoy(uint8_t cmd);               /* 带滤波的坐标读取(X/Y) */
static void tp_read_xy(uint16_t *x, uint16_t *y);       /* 双方向读取(X+Y) */
static uint8_t tp_read_xy2(uint16_t *x, uint16_t *y);   /* 带加强滤波的双方向坐标读取 */
static void tp_draw_touch_point(uint16_t x, uint16_t y, uint16_t color);    /* 画一个坐标校准点 */
static void tp_adjust_info_show(uint16_t xy[5][2], double px, double py);   /* 显示校准信息 */

void tp_draw_big_point(uint16_t x, uint16_t y, uint16_t color);	/* 画一个大点 */
void tp_save_adjust_data(void);				/* 保存校准参数 */
uint8_t tp_get_adjust_data(void);		  /* 读取校准参数 */
void tp_adjust(void);							    /* 触摸屏校准 */
uint8_t rtp_init(void);               /* 电阻触摸屏初始化 */
                        
uint8_t tp_scan(uint8_t mode);				/* 触摸扫描 */
uint8_t tp_init(void);							  /* 触摸屏初始化 */

#endif

















