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

#include "lvgl.h"
#include "gui_guider.h"
#include "events_init.h"

lv_ui guider_ui;

void HardWare_Init(void)
{
	Systick_Init(200);
	LED_Init();
    usart_init(2000000);
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
	
	setup_ui(&guider_ui);
  	events_init(&guider_ui);
	while(1)
	{
		lv_timer_handler();
		delay_ms(5);
	}
}
