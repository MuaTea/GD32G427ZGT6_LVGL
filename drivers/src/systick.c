#include "systick.h"

uint32_t nus;
uint32_t nms;

/**
 * @brief:systick初始化
 * @param:sysclk时钟频率（200MHz）
 * @retval:无
 */
void Systick_Init(uint16_t sysclk)
{
	SysTick->CTRL = 0;
	/* 选择时钟源 */
	SysTick->CTRL &= ~(1<<2);//选择200/8=25MHz工作频率
	
	nus = sysclk/8;
	nms = nus*1000;
}

/**
 * @brief:systick的微秒级延时
 * @param:n延时时间
 * @retval:无
 */
void delay_us(uint32_t n)
{
	uint32_t temp;
	
	SysTick->LOAD = n*nus;
	SysTick->VAL = 0;
	SysTick->CTRL |= 1;//启动systick
	
	do{
		temp = SysTick->CTRL;
	}while((temp & 1) && !(temp & (1<<16)));
	SysTick->CTRL &= ~1;
	SysTick->VAL = 0;
}

/**
 * @brief:systick的毫秒级延时
 * @param:n延时时间
 * @note:延时时间不得超过0.67秒，如果超过则使用函数delay_nms()
 * @retval:无
 */
void delay_ms(uint32_t n)
{
	uint32_t temp;
	SysTick->LOAD = n*nms;
	SysTick->VAL = 0;
	SysTick->CTRL |= 1;//启动systick
	
	do{
		temp = SysTick->CTRL;
	}while((temp & 1) && !(temp & (1<<16)));
	SysTick->CTRL &= ~1;
	SysTick->VAL = 0;
}

/**
 * @brief:systick的毫秒级延时
 * @param:n延时时间
 * @retval:无
 */
void delay_nms(uint16_t n)
{
	uint16_t round,remain,i;
	round = n/500;
	remain = n%500;
	
	for(i = 0;i < round;i++)
	{
		delay_ms(500);
	}
	if(remain > 0)
	{
		delay_ms(remain);
	}
}
