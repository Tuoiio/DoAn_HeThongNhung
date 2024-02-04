#ifndef _USART_H_
#define _USART_H_

#include <stm32f10x.h>
#include <stdio.h>

#if defined (STM32F10X_MD)
#define USE_USART1
#define USE_USART2
#define USE_USART3
#endif


#define USARTBUFFSIZE	32
typedef struct{
	uint8_t In;
	uint8_t Out;
	uint8_t Buff[USARTBUFFSIZE];
}FIFO_TypeDef;

void USART_Configuration(USART_TypeDef* pUSART, uint32_t BaudRate);

void USART_Config_IT(USART_TypeDef *pUSART, uint8_t Preemption, uint8_t Sub);

void USART_PutData(USART_TypeDef* pUSART, uint8_t* pTXBuffer, uint16_t Length);

void USART_GetData(USART_TypeDef* pUSART, uint8_t* pRXBuffer, uint16_t Length);

ErrorStatus Buffer_Get_IT(__IO FIFO_TypeDef *buffer, uint8_t ch);

ErrorStatus USART_Receive_IT(USART_TypeDef *pUSART, uint8_t *Rx_Buffer, uint8_t size);

#ifdef __STM32F10x_DMA_H
void USART_DMA_Configuration(USART_TypeDef *pUSART, uint32_t BaudRate);

void USART_DMA_Receive(USART_TypeDef *pUSART, uint8_t *Rx_Data, uint16_t Length);

void USART_DMA_Send(USART_TypeDef *pUSART, uint8_t *Tx_Data, uint16_t Length);
#endif
#endif
