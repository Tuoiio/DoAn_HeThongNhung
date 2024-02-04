#ifndef _PWM_H_
#define _PWM_H_

#include <stm32f10x.h>


typedef enum
{
	PWM_Channel_1,
	PWM_Channel_2,
	PWM_Channel_3,
	PWM_Channel_4
}PWM_Channel_t;

typedef enum 
{
	PWM_TIM_FREQ_1HZ = 1U,
	PWM_TIM_FREQ_10HZ = 2U,
	PWM_TIM_FREQ_100HZ = 3U,
	PWM_TIM_FREQ_1KHZ = 4U,
	PWM_TIM_FREQ_10KHZ = 5U
} PWM_TIM_Freq_t;


FlagStatus PWM_Init(PWM_TIM_Freq_t PWM_TIM_Freq, TIM_TypeDef* TIMx, PWM_Channel_t Channel);

static void TIM_OC(TIM_TypeDef* TIMx, PWM_Channel_t Channel);

static void PWM_InitChannel(TIM_TypeDef* TIMx, PWM_Channel_t Channel);

void PWM_SetCompare(TIM_TypeDef* TIMx, PWM_Channel_t Channel, uint16_t PWM);
/*
#ifdef __STM32F10x_DMA_H
static void DMA_InitChannel(TIM_TypeDef* TIMx, PWM_Channel_t Channel, uint32_t *pData, uint16_t Length);

FlagStatus PWM_DMA_Configruation(PWM_TIM_Freq_t PWM_TIM_Freq, TIM_TypeDef* TIMx, PWM_Channel_t Channel, uint32_t* pData, uint16_t Length);
#endif
*/
#endif
