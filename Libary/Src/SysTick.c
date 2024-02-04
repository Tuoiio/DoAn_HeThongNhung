#include "Delay_SysTick.h"


volatile uint32_t uwTick;


void SystemTick_Init(Tick_Freq_t TickFreq)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / TickFreq);
}

__weak void InTick(void)
{
	uwTick = uwTick + 1;
}

__weak uint32_t GetTick(void)
{
	return uwTick;
}

__weak void SysTick_Delay(uint16_t Delay)
{
	uint32_t tickStart = GetTick();
	while((GetTick() - tickStart) < Delay)
	{
	}
}

/*************************** Ham vecto ngat ************************************/
void SysTick_Handler(void)
{
	InTick();
}
