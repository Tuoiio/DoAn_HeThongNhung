#include "USART.h"


 FIFO_TypeDef Buffer_Rx1, Buffer_Rx2, Buffer_Rx3;


void Buffer_Init(__IO FIFO_TypeDef *Buffer)
{
	Buffer->In = 0;			// Index points to start
	Buffer->Out = 0;			// Index points to to start
}

	
	
ErrorStatus USART_Receive_IT(USART_TypeDef *pUSART, uint8_t *Rx_Buffer, uint8_t size)
{
	uint8_t ch;
	FIFO_TypeDef *buffer;
	if(pUSART == USART1){
		buffer = &Buffer_Rx1;
		ch = USART1->DR;
		
	} else if(pUSART == USART2) {
		buffer = &Buffer_Rx2;
		ch = USART2->DR;
		
	} else if(pUSART == USART3) {
		buffer = &Buffer_Rx3;
		ch = USART3->DR;
	
	} else {
		return ERROR;
	}
			
	buffer->Buff[buffer->In] = ch;
	Rx_Buffer[buffer->In] = (uint8_t)buffer->Buff[buffer->In];

	buffer->In++;
	if((buffer->Buff[buffer->In - 1] == 0x0A) || buffer->In >= size){
		buffer->In = 0;
	}
	return SUCCESS;
}




static void USART1_PinPack(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//Configure USART1 Tx (PA.09) as alternate function push-pull 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//Configure USART1 Rx (PA.10) as input floating
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void USART2_PinPack(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	//Configure USART2 Tx (PA.02) as alternate function push-pull 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//Configure USART2 Rx (PA.03) as input floating
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void USART3_PinPack(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	//Configure USART1 Tx (PB.10) as alternate function push-pull 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//Configure USART1 Rx (PB.11) as input floating
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void USART_Config_Para(USART_TypeDef *pUSART, uint32_t BaudRate)
{
	if(pUSART == USART1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	} else if (pUSART == USART2) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
	} else if (pUSART == USART3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	}
	USART_DeInit(pUSART);
	
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate= BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(pUSART, &USART_InitStructure);
	USART_Cmd(pUSART, ENABLE);
}


void USART_Configuration(USART_TypeDef* pUSART, uint32_t BaudRate)
{		
	if(pUSART == USART1) {
		USART1_PinPack();
		USART_Config_Para(USART1, BaudRate);
	
	} else if (pUSART == USART2){
		USART2_PinPack();
		USART_Config_Para(USART2, BaudRate);
	
	} else if (pUSART == USART3){
		USART3_PinPack();
		USART_Config_Para(USART3, BaudRate);

	}
	
	USART_Cmd(pUSART, ENABLE);

}

void USART_Config_IT(USART_TypeDef *pUSART, uint8_t Preemption, uint8_t Sub)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	if(pUSART == USART1) {
		NVIC_InitStructure.NVIC_IRQChannel =  USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =Preemption;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = Sub;
		Buffer_Init(&Buffer_Rx1);
	
	} else if(pUSART == USART2) {
		NVIC_InitStructure.NVIC_IRQChannel =  USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =Preemption;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = Sub;
		Buffer_Init(&Buffer_Rx2);
	
	} else if(pUSART == USART3) {
		NVIC_InitStructure.NVIC_IRQChannel =  USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Preemption;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = Sub;
		Buffer_Init(&Buffer_Rx3);
	}
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(pUSART, USART_IT_RXNE, ENABLE);
}
	
void USART_PutData(USART_TypeDef* pUSART, uint8_t* pTXBuffer, uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i++)
	{
		while(USART_GetFlagStatus(pUSART, USART_FLAG_TXE) == RESET);
		
		pUSART->DR = (uint8_t)(*pTXBuffer & 0xFF);
		
		pTXBuffer++;
	}
	while(USART_GetFlagStatus(pUSART,USART_FLAG_TC) == RESET);
}

void USART_GetData(USART_TypeDef* pUSART, uint8_t* pRXBuffer, uint16_t Length)
{
	uint16_t i ;
	for(i = 0; i < Length; i++)
	{
		while(USART_GetFlagStatus(pUSART, USART_FLAG_RXNE) == RESET);
		
		*pRXBuffer = (uint8_t)(pUSART->DR & 0xFF);
		
		pRXBuffer++;
	}
}




#ifdef __STM32F10x_DMA_H
/********************************************************************************USART DMA************************8*********************************************************/

void USART_DMA_Configuration(USART_TypeDef *pUSART, uint32_t BaudRate)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
	
	if(pUSART == USART1) {
			USART1_PinPack();
			USART_Config_Para(USART1, BaudRate);
		//     USART1 NVIC
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 15;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			
		} else if (pUSART == USART2){
			USART2_PinPack();
			USART_Config_Para(USART2, BaudRate);
			//     USART2 NVIC
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 14;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		
		} else if (pUSART == USART3){
			USART3_PinPack();
			USART_Config_Para(USART3, BaudRate);
			//     USART3 NVIC 
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 13;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}
		USART_ITConfig(pUSART, USART_IT_RXNE, DISABLE);
		USART_ITConfig(pUSART, USART_IT_TXE, DISABLE);
		USART_ITConfig(pUSART, USART_IT_TC, DISABLE);
		
		USART_ITConfig(pUSART, USART_IT_IDLE, ENABLE);
		USART_DMACmd(pUSART, USART_DMAReq_Rx, ENABLE);
}

void USART_DMA_Receive(USART_TypeDef *pUSART, uint8_t *Rx_Data, uint16_t Length)
{
	DMA_InitTypeDef DMA_InitStructure;
	if(pUSART == USART1){
		DMA_DeInit(DMA1_Channel5);
	
	} else if(pUSART == USART2) {
		DMA_DeInit(DMA1_Channel6);
		
	} else if(pUSART == USART3)  {
		DMA_DeInit(DMA1_Channel3);
	}
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&pUSART->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Rx_Data;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = Length;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;


	USART_DMACmd(pUSART, USART_DMAReq_Rx, ENABLE);
	
	if(pUSART == USART1){
		DMA_Init(DMA1_Channel5, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel5, ENABLE);
		// Disable IT_HT, because DMA create IT HalfReception when received hafl data.
		DMA_ITConfig(DMA1_Channel5, DMA_IT_HT, DISABLE);
		
	} else if(pUSART == USART2) {
		DMA_Init(DMA1_Channel6, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel6, ENABLE);
		// Disable IT_HT, because DMA create IT HalfReception when received hafl data.
		DMA_ITConfig(DMA1_Channel6, DMA_IT_HT, DISABLE);
		
	} else if(pUSART == USART3)  {
		DMA_Init(DMA1_Channel3, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel3, ENABLE);
		// Disable IT_HT, because DMA create IT HalfReception when received hafl data.
		DMA_ITConfig(DMA1_Channel3, DMA_IT_HT, DISABLE);
	}
}
	

void USART_DMA_Send(USART_TypeDef *pUSART, uint8_t *Tx_Data, uint16_t Length)
{
	DMA_InitTypeDef DMA_InitStructure;
	if(pUSART == USART1){
		DMA_DeInit(DMA1_Channel4);
	
	} else if(pUSART == USART2) {
		DMA_DeInit(DMA1_Channel7);
		
	} else if(pUSART == USART3)  {
		DMA_DeInit(DMA1_Channel2);
	}
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&pUSART->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Tx_Data;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = Length - 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	
	USART_DMACmd(pUSART, USART_DMAReq_Tx, ENABLE);
	
	if(pUSART == USART1){
		DMA_Init(DMA1_Channel4, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel4, ENABLE);
		// Disable IT_HT, because DMA create IT HalfReception when received hafl data.
		DMA_ITConfig(DMA1_Channel4, DMA_IT_HT, DISABLE);
		
	} else if(pUSART == USART2) {
		DMA_Init(DMA1_Channel7, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel7, ENABLE);
		// Disable IT_HT, because DMA create IT HalfReception when received hafl data.
		DMA_ITConfig(DMA1_Channel7, DMA_IT_HT, DISABLE);
		
	} else if(pUSART == USART3)  {
		DMA_Init(DMA1_Channel2, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel2, ENABLE);
		// Disable IT_HT, because DMA create IT HalfReception when received hafl data.
		DMA_ITConfig(DMA1_Channel2, DMA_IT_HT, DISABLE);
		
	}
}

#endif
