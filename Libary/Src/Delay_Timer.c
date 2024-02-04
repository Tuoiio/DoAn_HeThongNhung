#include "Delay_Timer.h"

//static TIM_TypeDef *gHtim;

void Delay_Timer_Init(void){
	/* Configure using libary */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	/* Enable TIMx Clock 
	if(htim == TIM1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	} else if( htim == TIM2) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	} else if( htim == TIM3) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	} else if(htim == TIM4) {
	*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//}
	/* TIMx Config */
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF-1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM4, ENABLE);
	//gHtim = htim;
}

void Delay_us(uint16_t us){
	TIM_SetCounter(TIM4, 0);
	while(TIM_GetCounter(TIM4) < us);
}

void Delay_ms(uint16_t ms){
	while(ms--){
		Delay_us(1000);
	}
}
	