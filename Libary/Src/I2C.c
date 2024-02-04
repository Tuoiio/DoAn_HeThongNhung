#include "I2C.h"


void I2C_GPIO_Config(I2C_TypeDef *pI2C)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	if(pI2C == I2C1) {
		// Pin 6: SCL        Pin 7: SDA
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	
	} else if(pI2C == I2C2) {
		// Pin 10: SCL		Pin 11: SDA
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	}
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2C_Para_Config(I2C_TypeDef *pI2C, I2C_Baud Baud)
{
	if(pI2C == I2C1){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	}else if(pI2C == I2C2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	}
	
	I2C_InitTypeDef I2C_InitStructure;

	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0 ; // 
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;       
	if(Baud == I2C_100KHZ){
		I2C_InitStructure.I2C_ClockSpeed = 100000;			
	} else if(Baud == I2C_400KHZ){
		I2C_InitStructure.I2C_ClockSpeed = 400000;
	}
    I2C_Init(pI2C, &I2C_InitStructure);
}

void I2C_Configuration(I2C_TypeDef *pI2C, I2C_Baud Baud)
{
	I2C_GPIO_Config(pI2C);
	I2C_Para_Config(pI2C,Baud);
}


void I2C_PutData(I2C_TypeDef *pI2C, uint8_t SlaveAddress, uint8_t *pTxBuffer, uint16_t Length)
{
	/* Send START Condition */
	I2C_GenerateSTART(pI2C, ENABLE);
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(pI2C, I2C_EVENT_MASTER_MODE_SELECT));
	/* Send Addrress for write */
	I2C_Send7bitAddress(pI2C, SlaveAddress, I2C_Direction_Transmitter);
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(pI2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	/* Send the byte to be written */
	while(Length > 0)
	{
		while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE));
		I2C_SendData(pI2C, *pTxBuffer);
		pTxBuffer++;
		Length--;
	}
	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(pI2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	/* Send STOP Condition */
	I2C_GenerateSTOP(pI2C, ENABLE);
}

void I2C_GetData(I2C_TypeDef *pI2C,uint8_t SlaveAddress, uint8_t *pRxBuffer, uint8_t Length)
{
	/* Send START condition */
	I2C_GenerateSTART(pI2C, ENABLE);
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(pI2C, I2C_EVENT_MASTER_MODE_SELECT));
	/* Send Addrress for write */
	I2C_Send7bitAddress(pI2C, SlaveAddress, I2C_Direction_Receiver);
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(pI2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	/* Read  byte from slave */
	while(Length) {
		if(Length == 1) {
			I2C_AcknowledgeConfig(pI2C, DISABLE);
			I2C_GenerateSTOP(pI2C, ENABLE);
		}
		while(I2C_CheckEvent(pI2C, I2C_EVENT_MASTER_BYTE_RECEIVED) == 0);
		
		*pRxBuffer = I2C_ReceiveData(pI2C);
		pRxBuffer++;
		Length--;
	}
	I2C_AcknowledgeConfig(pI2C, ENABLE);
}


