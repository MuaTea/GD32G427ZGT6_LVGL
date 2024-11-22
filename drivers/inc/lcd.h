#ifndef __LCD_H
#define __LCD_H

#include "stdlib.h"
#include <stdint.h>


/******************************************************************************************/

/* LCD��Ҫ������ */
typedef struct
{
    uint16_t width;     /* LCD ��� */
    uint16_t height;    /* LCD �߶� */
    uint16_t id;        /* LCD ID */
    uint8_t dir;        /* ���������������ƣ�0��������1�������� */
    uint16_t wramcmd;   /* ��ʼдgramָ�� */
    uint16_t setxcmd;   /* ����x����ָ�� */
    uint16_t setycmd;   /* ����y����ָ�� */
} _lcd_dev;

/* LCD���� */
extern _lcd_dev lcddev; /* ����LCD��Ҫ���� */

/* LCD�Ļ�����ɫ�ͱ���ɫ */
extern uint16_t  g_point_color;     /* Ĭ�Ϻ�ɫ */
extern uint16_t  g_back_color;      /* ������ɫ.Ĭ��Ϊ��ɫ */

/* LCD������� */
#define LCD_BL(x)        do{ x ? \
                             gpio_bit_write(GPIOB, GPIO_PIN_15, SET) : \
                             gpio_bit_write(GPIOB, GPIO_PIN_15, RESET); \
                         }while(0)     

/* LCD��λ���� */
#define LCD_RST(x)       do{ x ? \
                             gpio_bit_write(GPIOG, GPIO_PIN_6, SET) : \
                             gpio_bit_write(GPIOG, GPIO_PIN_6, RESET); \
                         }while(0)     
/* LCD��ַ�ṹ�� */
typedef struct
{
    volatile uint16_t LCD_REG;
    volatile uint16_t LCD_RAM;
} LCD_TypeDef;


/******************************************************************************************/
/* EXMC��ز��� ���� 
 * ע��: ����Ĭ����ͨ��EXMC��0������LCD, ��0��4��Ƭѡ: EXMC_NE0~3
 *
 */
#define LCD_EXMC_NEX         3              /* ʹ��EXMC_NE3��LCD_CS,ȡֵ��Χֻ����: 0~3 */
#define LCD_EXMC_AX          6              /* ʹ��EXMC_A6��LCD_RS,ȡֵ��Χ��: 0 ~ 25 */

/* LCD_BASE����ϸ���㷽��:
 * ����һ��ʹ��EXMC�Ŀ�0(BANK0)������TFTLCDҺ����(MCU��), ��0��ַ��Χ�ܴ�СΪ256MB,���ֳ�4��:
 * �洢��0(EXMC_NE0)��ַ��Χ: 0X6000 0000 ~ 0X63FF FFFF
 * �洢��1(EXMC_NE1)��ַ��Χ: 0X6400 0000 ~ 0X67FF FFFF
 * �洢��2(EXMC_NE2)��ַ��Χ: 0X6800 0000 ~ 0X6BFF FFFF
 * �洢��3(EXMC_NE3)��ַ��Χ: 0X6C00 0000 ~ 0X6FFF FFFF
 *
 * ������Ҫ����Ӳ�����ӷ�ʽѡ����ʵ�Ƭѡ(����LCD_CS)�͵�ַ��(����LCD_RS)
 * ������ʹ��EXMC_NE3����LCD_CS, EXMC_A6����LCD_RS ,16λ������,���㷽������:
 * ����EXMC_NE3�Ļ���ַΪ: 0X6C00 0000;     NEx�Ļ�ַΪ(x=0/1/2/3): 0X6000 0000 + (0X400 0000 * x)
 * EXMC_A6��Ӧ��ֵַ: 2^6 * 2 = 0X80;       EXMC_Ay��Ӧ�ĵ�ַΪ(y = 0 ~ 25): 2^y * 2
 *
 * LCD->LCD_REG,��ӦLCD_RS = 0(LCD�Ĵ���); LCD->LCD_RAM,��ӦLCD_RS = 1(LCD����)
 * �� LCD->LCD_RAM�ĵ�ַΪ:  0X6C00 0000 + 2^6 * 2 = 0X6C00 0080
 *    LCD->LCD_REG�ĵ�ַ����Ϊ LCD->LCD_RAM֮��������ַ.
 * ��������ʹ�ýṹ�����LCD_REG �� LCD_RAM(REG��ǰ,RAM�ں�,��Ϊ16λ���ݿ��)
 * ��� �ṹ��Ļ���ַ(LCD_BASE) = LCD_RAM - 2 = 0X6C00 0080 -2
 *
 * ����ͨ�õļ��㹫ʽΪ((Ƭѡ��EXMC_NEx)x=0/1/2/3, (RS�ӵ�ַ��EXMC_Ay)y=0~25):
 *          LCD_BASE = (0X6000 0000 + (0X400 0000 * x)) | (2^y * 2 -2)
 *          ��Ч��(ʹ����λ����)
 *          LCD_BASE = (0X6000 0000 + (0X400 0000 * x)) | ((1 << y) * 2 -2)
 */
#define LCD_BASE        (uint32_t)((0X60000000 + (0X4000000 * LCD_EXMC_NEX)) | (((1 << LCD_EXMC_AX) * 2) -2))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

/******************************************************************************************/
/* LCDɨ�跽�����ɫ ���� */

/* ɨ�跽���� */
#define L2R_U2D         0           /* ������,���ϵ��� */
#define L2R_D2U         1           /* ������,���µ��� */
#define R2L_U2D         2           /* ���ҵ���,���ϵ��� */
#define R2L_D2U         3           /* ���ҵ���,���µ��� */

#define U2D_L2R         4           /* ���ϵ���,������ */
#define U2D_R2L         5           /* ���ϵ���,���ҵ��� */
#define D2U_L2R         6           /* ���µ���,������ */
#define D2U_R2L         7           /* ���µ���,���ҵ��� */

#define DFT_SCAN_DIR    L2R_U2D     /* Ĭ�ϵ�ɨ�跽�� */

/* ���û�����ɫ */
#define WHITE           0xFFFF      /* ��ɫ */
#define BLACK           0x0000      /* ��ɫ */
#define RED             0xF800      /* ��ɫ */
#define GREEN           0x07E0      /* ��ɫ */
#define BLUE            0x001F      /* ��ɫ */ 
#define MAGENTA         0XF81F      /* Ʒ��ɫ/�Ϻ�ɫ = BLUE + RED */
#define YELLOW          0XFFE0      /* ��ɫ = GREEN + RED */
#define CYAN            0X07FF      /* ��ɫ = GREEN + BLUE */  

/* �ǳ�����ɫ */
#define BROWN           0XBC40      /* ��ɫ */
#define BRRED           0XFC07      /* �غ�ɫ */
#define GRAY            0X8430      /* ��ɫ */ 
#define DARKBLUE        0X01CF      /* ����ɫ */
#define LIGHTBLUE       0X7D7C      /* ǳ��ɫ */ 
#define GRAYBLUE        0X5458      /* ����ɫ */ 
#define LIGHTGREEN      0X841F      /* ǳ��ɫ */  
#define LGRAY           0XC618      /* ǳ��ɫ(PANNEL),���屳��ɫ */ 
#define LGRAYBLUE       0XA651      /* ǳ����ɫ(�м����ɫ) */ 
#define LBBLUE          0X2B12      /* ǳ����ɫ(ѡ����Ŀ�ķ�ɫ) */ 
   
/******************************************************************************************/
/* �������� */
  
void lcd_wr_data(volatile uint16_t data);            /* LCDд���� */
void lcd_wr_regno(volatile uint16_t regno);          /* LCDд�Ĵ������/��ַ */
void lcd_write_reg(uint16_t regno, uint16_t data);   /* LCDд�Ĵ�����ֵ */

void lcd_init(void);                        /* ��ʼ��LCD */ 
void exmc_sram_init(void);                  /* ��ʼ��EXMC */  
void lcd_display_on(void);                  /* ����ʾ */ 
void lcd_display_off(void);                 /* ����ʾ */
void lcd_scan_dir(uint8_t dir);             /* ������Ļɨ�跽�� */ 
void lcd_display_dir(uint8_t dir);          /* ������Ļ��ʾ���� */ 

void lcd_write_ram_prepare(void);                               /* ׼��дGRAM */ 
void lcd_set_cursor(uint16_t x, uint16_t y);                    /* ���ù�� */ 
uint16_t lcd_read_point(uint16_t x, uint16_t y);                /* ���� */
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);    /* ���� */

void lcd_clear(uint16_t color);                                                                 /* LCD���� */
void lcd_fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);                       /* ���ʵ��Բ */
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);                      /* ��Բ */
void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color);                      /* ��ˮƽ�� */
void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);                 /* ���ô��� */
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);              /* ��ɫ������ */
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);       /* ��ɫ������ */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);         /* ��ֱ�� */
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);    /* ������ */

void lcd_show_char(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode, uint16_t color);                       /* ��ʾһ���ַ� */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color);                     /* ��ʾ���� */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color);      /* ��չ��ʾ���� */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color);   /* ��ʾ�ַ��� */


#endif

















