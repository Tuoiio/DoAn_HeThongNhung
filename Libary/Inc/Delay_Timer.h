#ifndef _DELAY_H_
#define _DELAY_H_

#include <stm32f10x.h>
void Delay_Timer_Init(void);

void Delay_us(uint16_t us);

void Delay_ms(uint16_t ms);

#endif
