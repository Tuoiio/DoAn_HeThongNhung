#include "SPI.h"
/*
#define SPI_SIZE 32
typedef struct {
	uint8_t pData[SPI_SIZE];
	uint8_t Size;
	uint8_t In;
} SPI_FIFO;

SPI_FIFO SPI1_FIFO, SPI2_FIFO;

 void SPI_IT_GetData(SPI_TypeDef *pSPI, uint8_t *pData, uint16_t Length)
{
	SPI_FIFO *pSPI_FIFO;
	uint8_t ch;
	if(pSPI == SPI1) {
		pSPI_FIFO = &SPI1_FIFO;
		ch = SPI1->DR;
	
	} else if( pSPI == SPI2) {
		pSPI_FIFO = &SPI2_FIFO;
		ch = SPI2->DR;
	}
	pSPI_FIFO->pData[pSPI_FIFO->In] = ch;
	pData[pSPI_FIFO->In] = pSPI_FIFO->pData[pSPI_FIFO->In];
	pSPI_FIFO->In++;
	
	if(pSPI_FIFO->In >= Length) {
		pSPI_FIFO->In = 0;
	}
}
	
static void SPI_Config_Pin(SPI_TypeDef *pSPI, SPI_Mode Mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(pSPI == SPI1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
		if( Mode == SPI_Master) {
			// Configuration pin PA7 MOSI AF_PP
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			// Configuration pin PA6 MISO INPUT FLOATING
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			// Configuration pin PA5 CLK AF_PP
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			// Configuration pin PA4 CS OUT_PP
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		} else if (Mode == SPI_Slave) {
			// Configuration pin PA7 MOSI IN FLOATING
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			// Configuration pin PA6 MISO AF_PP
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			// Configuration pin PA5 CLK INPUT FLOATING
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			// Configuration pin PA4 CS  INPUT FLOATING
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		}
		
	} else if( pSPI == SPI2) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
		if( Mode == SPI_Master) {
			// Configuration pin PB15 MOSI AF_PP
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			// Configuration pin PB14 MISO INPUT FLOATING
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			// Configuration pin PB13 CLK AF_PP
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			// Configuration pin PB12 CS OUT_PP
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		} else if (Mode == SPI_Slave) {
			// Configuration pin PB15 MOSI IN FLOATING
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			// Configuration pin PB14 MISO AF_PP
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			// Configuration pin PB13 CLK INPUT FLOATING
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			// Configuration pin PB12 CS  INPUT FLOATING
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		}	
	}
}


static void SPI_Config_Data(SPI_TypeDef *pSPI, SPI_Mode Mode)
{
	SPI_InitTypeDef SPI_InitStructure;
	
	if(pSPI == SPI1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  // APB2 = 72Mhz => SPI1 = 72/4= 18MBit /s
	} else if(pSPI == SPI2) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  // APB2 = 36Mhz => SPI1 = 36/2= 18MBit /s
	}
	
	if(Mode == SPI_Master) {
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	} else if (Mode == SPI_Slave) {
		SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	}
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	//SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(pSPI, &SPI_InitStructure);
	SPI_Cmd(pSPI, ENABLE);
}
	
		
		

 void SPI_Configuration(SPI_TypeDef *pSPI, SPI_Mode Mode)
{
	SPI_Config_Pin(pSPI, Mode);
	SPI_Config_Data(pSPI, Mode);
}

 void SPI_IT_Configuration(SPI_TypeDef *pSPI, uint8_t Preemption, uint8_t Sub )
{
	NVIC_InitTypeDef NVIC_InitStrcture;
	if(pSPI == SPI1){
		NVIC_InitStrcture.NVIC_IRQChannel = SPI1_IRQn;
	
	} else if( pSPI == SPI2) {
		NVIC_InitStrcture.NVIC_IRQChannel = SPI2_IRQn;
	}
	NVIC_InitStrcture.NVIC_IRQChannelPreemptionPriority = Preemption;
	NVIC_InitStrcture.NVIC_IRQChannelSubPriority = Sub;
	NVIC_InitStrcture.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStrcture);
}
*/
 //void SPI_DMA_Configuration(SPI_TypeDef *pSPI, SPI_Mode Mode){
 void SPI_DMA_Configuration(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	 DMA_InitTypeDef DMA_InitStructure;
	 DMA_DeInit(DMA1_Channel3);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = 0;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 0 ;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;//
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/* Enable DMA Stream Transfer Complete interrupt */
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC , ENABLE);
	DMA_ITConfig(DMA1_Channel3, DMA_IT_HT, DISABLE);


	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the Priority Group to 2 bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable the SPI1 TX DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 14;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

 void SPI_DMA_Receive(SPI_TypeDef *pSPI, uint8_t *pData, uint16_t  Length)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	if(pSPI == SPI1) DMA_DeInit(DMA1_Channel2);
	else if(pSPI == SPI2) DMA_DeInit(DMA1_Channel4);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&pSPI->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pData;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = Length ;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	if(pSPI == SPI1){
		DMA_Init(DMA1_Channel2, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel2, ENABLE);
	}
	
	else if(pSPI == SPI2){
		DMA_Init(DMA1_Channel4, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel4, ENABLE);
	}
	
	SPI_I2S_DMACmd(pSPI, SPI_I2S_DMAReq_Rx, ENABLE);
}


 void SPI_DMA_Send(SPI_TypeDef *pSPI, uint8_t *pData, uint16_t Length)
{
	DMA1_Channel3->CCR &= (uint16_t)(~DMA_CCR3_EN);		// DISABLE DMA
	DMA1_Channel3->CPAR =  (uint32_t)&pSPI->DR;
	DMA1_Channel3->CMAR = (uint32_t)pData;
	DMA1_Channel3->CNDTR = Length;
	DMA1_Channel3->CCR = DMA_DIR_PeripheralDST | DMA_PeripheralInc_Disable | DMA_MemoryInc_Enable | DMA_PeripheralDataSize_Byte |
										DMA_MemoryDataSize_Byte | DMA_Priority_Low | DMA_M2M_Disable | DMA_CCR3_EN;
}

 void SPI_PutData(SPI_TypeDef *pSPI, uint8_t* pData, uint16_t Length)
{
	for(uint16_t i = 0; i < Length; i++)
	{
		// 1.wait until TXE is set
		while(SPI_I2S_GetFlagStatus(pSPI, SPI_I2S_FLAG_TXE) == RESET);
		
		// 2. check the DFF bit in CR1
		if(pSPI->CR1 &  SPI_CR1_DFF) {
			// 16bit DFF
			// 1.Load the data in to the DR
			pSPI->DR = *((uint16_t*)pData);
			Length++;
			(uint16_t *)pData++;
		
		} else {
			// 8 bit DFF
			pSPI->DR = *pData;
			pData++;
		}
	}
}

 void SPI_GetData(SPI_TypeDef *pSPI, uint8_t *pData, uint16_t Length)
{
	
	for(uint16_t i = 0; i < Length; i++){
		pSPI->DR = 0;  // send dummy data
		while (!((pSPI->SR) & (1 << 0)));
		if(pSPI->CR1 & SPI_CR1_DFF) {
			// 16 bit DFF
			*(uint16_t*)pData = pSPI->DR;
			(uint16_t *)pData++;
		} else {
			// 8 bit DFF
			*pData++ = pSPI->DR;
			//pData++;
			
		}
	}
}  

// Khi dung transmit thong qua DMA bat buoc phai xoa co TC khi moi dung dc tiep
// Neu dung nhan DMA thi bat co nhan cua kenh do 
 void DMA1_Channel3_IRQHandler(void) // SPI1_TX
{
	/* Test on DMA Transfer Complete interrupt */
	if (DMA_GetITStatus(DMA1_IT_TC3)){
		/* Clear DMA Transfer Complete interrupt pending bit */
		DMA_ClearITPendingBit(DMA1_IT_TC3);
	}
}

 void DMA1_Channel5_IRQHandler(void) // SPI2_TX
{
	/* Test on DMA Transfer Complete interrupt */
	if (DMA_GetITStatus(DMA1_IT_TC5)){
		/* Clear DMA Transfer Complete interrupt pending bit */
		DMA_ClearITPendingBit(DMA1_IT_TC5);
	}
}
