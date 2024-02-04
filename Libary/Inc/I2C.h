#ifndef _I2C_H_
#define _I2C_H_

#include <stm32f10x.h>

typedef enum
{
	I2C_100KHZ = 0u,
	I2C_400KHZ
}I2C_Baud;

void I2C_Configuration(I2C_TypeDef *pI2C, I2C_Baud Baud);

void I2C_PutData(I2C_TypeDef *pI2C, uint8_t SlaveAddress, uint8_t *pTxBuffer, uint16_t Length);

void I2C_GetData(I2C_TypeDef *pI2C,uint8_t SlaveAddress, uint8_t *pRxBuffer, uint8_t Length);

#endif
