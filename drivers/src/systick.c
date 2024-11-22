#include "systick.h"

uint32_t nus;
uint32_t nms;

/**
 * @brief:systick��ʼ��
 * @param:sysclkʱ��Ƶ�ʣ�200MHz��
 * @retval:��
 */
void Systick_Init(uint16_t sysclk)
{
	SysTick->CTRL = 0;
	/* ѡ��ʱ��Դ */
	SysTick->CTRL &= ~(1<<2);//ѡ��200/8=25MHz����Ƶ��
	
	nus = sysclk/8;
	nms = nus*1000;
}

/**
 * @brief:systick��΢�뼶��ʱ
 * @param:n��ʱʱ��
 * @retval:��
 */
void delay_us(uint32_t n)
{
	uint32_t temp;
	
	SysTick->LOAD = n*nus;
	SysTick->VAL = 0;
	SysTick->CTRL |= 1;//����systick
	
	do{
		temp = SysTick->CTRL;
	}while((temp & 1) && !(temp & (1<<16)));
	SysTick->CTRL &= ~1;
	SysTick->VAL = 0;
}

/**
 * @brief:systick�ĺ��뼶��ʱ
 * @param:n��ʱʱ��
 * @note:��ʱʱ�䲻�ó���0.67�룬���������ʹ�ú���delay_nms()
 * @retval:��
 */
void delay_ms(uint32_t n)
{
	uint32_t temp;
	SysTick->LOAD = n*nms;
	SysTick->VAL = 0;
	SysTick->CTRL |= 1;//����systick
	
	do{
		temp = SysTick->CTRL;
	}while((temp & 1) && !(temp & (1<<16)));
	SysTick->CTRL &= ~1;
	SysTick->VAL = 0;
}

/**
 * @brief:systick�ĺ��뼶��ʱ
 * @param:n��ʱʱ��
 * @retval:��
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
