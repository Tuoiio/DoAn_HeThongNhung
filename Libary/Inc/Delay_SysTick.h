#ifndef  _DELAY_SYSTICK_H_
#define _DELAY_SYSTICK_H_

#include <stm32f10x.h>

typedef enum
{
	SYSTICK_1MS  = 1000U,
	SYSTICK_10US = 100000U,
	SYSTICK_1US	= 1000000U
} Tick_Freq_t;

extern volatile uint32_t uwTick;


extern void InTick(void);
uint32_t GetTick(void);
void SystemTick_Init(Tick_Freq_t TickFreq);
void SysTick_Delay(uint16_t Delay);

#endif
