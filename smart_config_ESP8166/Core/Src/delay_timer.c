#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "delay_timer.h"

#define DELAY_1MS_COUNTER 20

extern TIM_HandleTypeDef htim3;

void delay_1ms(void)
{
	__HAL_TIM_SetCounter(&htim3, 0);
	while (__HAL_TIM_GetCounter(&htim3) < DELAY_1MS_COUNTER);
}

void delay_ms(uint32_t time)
{
	uint32_t counter = 0;
	
	for(counter = 0; counter < time; counter ++)
	{
		delay_1ms();
	}
}
