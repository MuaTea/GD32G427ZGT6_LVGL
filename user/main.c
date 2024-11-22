#include "timer_drv.h"
#include "systick.h"
#include "led_drv.h"
#include "usart_drv.h"
#include "lcd.h"
#include "key_drv.h"
#include "touch.h"
#include "sram.h"

#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"

void HardWare_Init(void)
{
	Systick_Init(200);
	LED_Init();
    usart_init(115200);
	timer6_init(1000-1,100-1);//1ms定时
	lcd_init();
	tp_dev.init(); 
	sram_init();                            //初始化外部SRAM 
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
}

int main(void)
{
	HardWare_Init();
	
	lv_obj_t* switch_obj = lv_switch_create(lv_scr_act());
	lv_obj_set_size(switch_obj,120,60);
	lv_obj_align(switch_obj,LV_ALIGN_CENTER,0,0); 	
	while(1)
	{
		lv_timer_handler();
		delay_ms(5);
	}
}
