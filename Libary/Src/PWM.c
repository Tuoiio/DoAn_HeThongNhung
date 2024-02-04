#include "PWM.h"
/*
 * PWM pins 
 *
 * 	TIMER	|	CHANNEL 1	|	CHANNEL 2	|	CHANNEL 3	|	CHANNEL 4	|	
 *	TIM 1	|		 PA8			|		 PA9			|		 PA10		|		 PA11		|				
 * 	TIM 2	|		 PA0			| 		 PA1			|		 PA2			|		 PA3			|
 * 	TIM 3	|		 PA6			|		 PA7			|		 PB0			|		 PB1			|
 * 	TIM 4	|		 PB6			|		 PB7			|		 PB8			|		 PB9			|
*/
//////////////////////////////////////////////////////////////////////////////////	 

static uint8_t	Tim1_Freq, Tim2_Freq, Tim3_Freq, Tim4_Freq;

void PWM_SetCompare(TIM_TypeDef *TIMx, PWM_Channel_t Channel, uint16_t PWM)
{
	uint8_t TIM_Freq_t;
	uint16_t PWM_t;
	
	if(TIMx == TIM1) 		TIM_Freq_t = Tim1_Freq;
	else if(TIMx == TIM2)	TIM_Freq_t = Tim2_Freq;
	else if(TIMx == TIM3) 	TIM_Freq_t = Tim3_Freq;
	else if(TIMx == TIM4) 	TIM_Freq_t = Tim4_Freq;
	
	switch(TIM_Freq_t) {
		case PWM_TIM_FREQ_1HZ:
			PWM_t = PWM * 100;
			break;
		case PWM_TIM_FREQ_10HZ:
			PWM_t = PWM * 10;
			break;
		case PWM_TIM_FREQ_100HZ:
		case PWM_TIM_FREQ_1KHZ:
		case PWM_TIM_FREQ_10KHZ:
			PWM_t = PWM;
			break;
		default:
			break;
	}

	switch (Channel){
		case PWM_Channel_1:
				TIM_SetCompare1(TIMx, PWM_t);
				break;
		case PWM_Channel_2:
				TIM_SetCompare2(TIMx, PWM_t);
				break;
		case PWM_Channel_3:
				TIM_SetCompare3(TIMx, PWM_t);
				break;
		case PWM_Channel_4:
				TIM_SetCompare4(TIMx, PWM_t);
				break;
		default:
				break;
	}
}

void PWM_InitChannel(TIM_TypeDef* TIMx, PWM_Channel_t Channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	if(TIMx == TIM1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		 switch (Channel) {
			case PWM_Channel_1:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			
			case PWM_Channel_2:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			
			case PWM_Channel_3:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			
			case PWM_Channel_4:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
		}
		 
	//-----------------------------------------------------TIM2-----------------------------------------------------//
	} else if( TIMx == TIM2) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		switch (Channel) {
			case PWM_Channel_1:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			
			case PWM_Channel_2:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			
			case PWM_Channel_3:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			
			case PWM_Channel_4:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
		}
		
	//-----------------------------------------------------TIM3-----------------------------------------------------//
	} else if( TIMx == TIM3) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		switch (Channel) {
			case PWM_Channel_1:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			
			case PWM_Channel_2:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				break;
			
			case PWM_Channel_3:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				break;
			
			case PWM_Channel_4:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				break;
		}
		
	//-----------------------------------------------------TIM4-----------------------------------------------------//
	} else if( TIMx == TIM4) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		switch (Channel) {
			case PWM_Channel_1:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				break;
			
			case PWM_Channel_2:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				break;
			
			case PWM_Channel_3:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				break;
			
			case PWM_Channel_4:
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				break;
		}
	}
}
		
void TIM_OC(TIM_TypeDef *TIMx, PWM_Channel_t Channel)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	switch(Channel) {
		case PWM_Channel_1:
			TIM_OC1Init(TIMx, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
			break;
		case PWM_Channel_2:
			TIM_OC2Init(TIMx, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
			break;
		case PWM_Channel_3:
			TIM_OC3Init(TIMx, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
			break;
		case PWM_Channel_4:
			TIM_OC4Init(TIMx, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
			break;
		default:
			break;
	}
}


FlagStatus PWM_Init(PWM_TIM_Freq_t PWM_TIM_Freq, TIM_TypeDef* TIMx, PWM_Channel_t Channel)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	uint16_t Prescaler = 0, Period = 0;
	FlagStatus bitstatus = RESET;
	
	if(PWM_TIM_Freq == PWM_TIM_FREQ_1HZ)			{ Prescaler = 7200, Period = 10000; }
	else if( PWM_TIM_Freq == PWM_TIM_FREQ_10HZ) 	{ Prescaler = 7200, Period = 1000; }
	else if( PWM_TIM_Freq == PWM_TIM_FREQ_100HZ) 	{ Prescaler = 7200, Period = 100; }
	else if( PWM_TIM_Freq == PWM_TIM_FREQ_1KHZ) 	{ Prescaler = 720,   Period = 100; }
	else if( PWM_TIM_Freq == PWM_TIM_FREQ_10KHZ)	{ Prescaler = 72,     Period = 100; }
	else { 
		bitstatus = SET;
		return bitstatus;
	}

	if(TIMx == TIM1) 		Tim1_Freq = PWM_TIM_Freq;
	else if( TIMx == TIM2) Tim2_Freq = PWM_TIM_Freq;
	else if( TIMx == TIM3) Tim3_Freq = PWM_TIM_Freq;
	else if( TIMx == TIM4) Tim4_Freq = PWM_TIM_Freq;
	else {
		bitstatus = SET;
		return bitstatus;
	}
						
	PWM_InitChannel(TIMx, Channel);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler - 1;
	TIM_TimeBaseInitStructure.TIM_Period =	Period - 1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure);
	
	TIM_OC(TIMx, Channel);
	
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	
	TIM_Cmd(TIMx, ENABLE);
	if(TIMx == TIM1)
	{
		TIM_CtrlPWMOutputs(TIM1, ENABLE);
	}
	return bitstatus;
}
/*
#ifdef __STM32F10x_DMA_H
static void DMA_InitChannel(TIM_TypeDef* TIMx, PWM_Channel_t Channel, uint32_t *pData, uint16_t Length)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	DMA_InitTypeDef DMA_InitStructure;
	
	switch(Channel) {
		case PWM_Channel_1:
			DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIMx->CCR1;			// TIMx Channel 1
			break;
		case PWM_Channel_2:
			DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIMx->CCR2;			// TIMx Channel 2
			break;
		case PWM_Channel_3:
			DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIMx->CCR3; 			// TIMx Channel 3
			break;
		case PWM_Channel_4:
			DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIMx->CCR4; 			// TIMx Channel 4
			break;
		default:
			break;
	}
	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pData;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = Length;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	if(TIMx == TIM1) {
		DMA_Init(DMA1_Channel5, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel5, ENABLE);
	
	} else if (TIMx == TIM2) {
		DMA_Init(DMA1_Channel2, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel2, ENABLE);
	
	} else if(TIMx == TIM3) {
		DMA_Init(DMA1_Channel3, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel3, ENABLE);
	
	} else if(TIMx == TIM4) {
		DMA_Init(DMA1_Channel7, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel7, ENABLE);
	}
}



FlagStatus PWM_DMA_Configruation(PWM_TIM_Freq_t PWM_TIM_Freq, TIM_TypeDef* TIMx, PWM_Channel_t Channel, uint32_t* pData, uint16_t Length)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	uint16_t Prescaler = 0, Period = 0;
	FlagStatus bitstatus = RESET;
	
	if(PWM_TIM_Freq == PWM_TIM_FREQ_1HZ)			{ Prescaler = 7200, Period = 10000; }
	else if( PWM_TIM_Freq == PWM_TIM_FREQ_10HZ) 	{ Prescaler = 7200, Period = 1000; }
	else if( PWM_TIM_Freq == PWM_TIM_FREQ_100HZ) 	{ Prescaler = 7200, Period = 100; }
	else if( PWM_TIM_Freq == PWM_TIM_FREQ_1KHZ) 	{ Prescaler = 720,   Period = 100; }
	else if( PWM_TIM_Freq == PWM_TIM_FREQ_10KHZ)	{ Prescaler = 72,     Period = 100; }
	else { 
		bitstatus = SET;
		return bitstatus;
	}

	if(TIMx == TIM1) 		Tim1_Freq = PWM_TIM_Freq;
	else if( TIMx == TIM2) Tim2_Freq = PWM_TIM_Freq;
	else if( TIMx == TIM3) Tim3_Freq = PWM_TIM_Freq;
	else if( TIMx == TIM4) Tim4_Freq = PWM_TIM_Freq;
	else {
		bitstatus = SET;
		return bitstatus;
	}
						
	PWM_InitChannel(TIMx, Channel);
	DMA_InitChannel(TIMx, Channel, pData, Length);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler - 1;
	TIM_TimeBaseInitStructure.TIM_Period =	Period - 1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStructure);
	
	TIM_OC(TIMx, Channel);
	
	TIM_DMACmd(TIMx, TIM_DMA_Update, ENABLE);
	TIM_Cmd(TIMx, ENABLE);
	

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
	return bitstatus;
}
#endif
*/

