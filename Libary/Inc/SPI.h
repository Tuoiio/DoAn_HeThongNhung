#ifndef _SPI_H_
#define _SPI_H_

#include <stm32f10x.h>

typedef  enum
{
	SPI_Master =  0U,
	SPI_Slave
} SPI_Mode;


void SPI_IT_GetData(SPI_TypeDef *pSPI, uint8_t *pData, uint16_t Length);

  void SPI_Configuration(SPI_TypeDef *pSPI, SPI_Mode Mode);

  void SPI_IT_Configuration(SPI_TypeDef *pSPI, uint8_t Preemption, uint8_t Sub);

 void SPI_PutData(SPI_TypeDef *pSPI, uint8_t* pData, uint16_t Length);

 void SPI_GetData(SPI_TypeDef *pSPI, uint8_t *pData, uint16_t Length);

 //void SPI_DMA_Configuration(SPI_TypeDef *pSPI, SPI_Mode Mode);
 void SPI_DMA_Configuration(void);

 void SPI_DMA_Receive(SPI_TypeDef *pSPI, uint8_t *pData, uint16_t  Length);

 void SPI_DMA_Send(SPI_TypeDef *pSPI, uint8_t *pData, uint16_t Length);

#endif
