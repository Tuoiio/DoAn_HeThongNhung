/*****************************************************************************************************************
Ten Thu Vien: ST7735 LIB
Chuc Nang: Dieu khien man hinh TFT LCD 1.44, 1.8 inch
Huong Dan Su dung: 
- Sua lai file Platform.h khi su dung voi thu vien khac nhau  
- Dinh Nghia cac chan CS, DC, RS la cac chan ouput
- Cau Hinh SPI, Sua lai marco de dung voi SPI can dung: #define ST7735_SPI_PORT SPI1
Pinout:

******************************************************************************************************************/
#include "ST7735_TFT_SPI.h"



static void ST7735_SPI_Trans(uint8_t *pBuff, uint16_t buff_size)
{		
	SPI_DMA_Send(SPI1, pBuff,buff_size);
	while(SPI1->SR & SPI_SR_BSY);
}

static void ST7735_delay(uint16_t time)
{
	Delay_ms(time);
}


 void ST7735_Select(void)
{
	//GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
	GPIOA->ODR &= ~(1 << 4);
}

 void ST7735_Unselect(void)
{
//	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
	GPIOA->ODR |= (1 << 4);
}

 void ST7735_Cmd(void)
{
	//GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
	GPIOA->ODR &= ~(1 << 2);
}

 void ST7735_DATA(void)
{
	//GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
	GPIOA->ODR |= (1 << 2);
}
 void ST7735_RESET_ON(void)
{
//	GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);
	GPIOA->ODR &= ~(1 << 3);
	
}

 void ST7735_RESET_OFF(void)
{
	//GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);
	GPIOA->ODR |= (1 << 3);
}






/************************************ Middle Level Functions ************************************************/



static void ST7735_Reset(void)
{
	ST7735_RESET_ON();
	ST7735_delay(5);
	ST7735_RESET_OFF();
}	

static void ST7735_WriteCommand(uint8_t cmd)
{
	ST7735_Cmd();
	ST7735_SPI_Trans(&cmd, sizeof(cmd));
}

static void ST7735_WriteData(uint8_t *pBuff, uint16_t buff_size)
{
	ST7735_DATA();
	ST7735_SPI_Trans(pBuff,  buff_size);
}


static void ST7735_ExecuteCommandList(const uint8_t *pAddress)
{
	uint8_t numCommands, numArgs;
	uint16_t ms;
	numCommands = *pAddress++;
	while(numCommands--) {
		uint8_t 	cmd = *pAddress++;
		//ST7735_WriteCommand_Init(cmd);
		// Neu dung DMA truyen dan, thi khi khoi tao TFT phai dung SPI thuong de cho ham delay
		ST7735_Cmd();
		SPI_PutData(ST7735_SPI_PORT, (uint8_t*)&cmd, sizeof(cmd));
		numArgs = *pAddress++;
		// If high bit set, delay follows args
		ms = numArgs & DELAY;
		numArgs &= ~DELAY;
		if(numArgs) {
			//Neu dung DMA truyen dan, thi khi khoi tao TFT phai dung SPI thuong de cho ham delay
			ST7735_DATA();
			SPI_PutData(ST7735_SPI_PORT, (uint8_t*)pAddress, numArgs);
			pAddress += numArgs;
		}
		if(ms) {
			ms = *pAddress++;
			if(ms == 255) ms = 500;
			ST7735_delay(ms);
		}
	}
}


static void ST7735_SetAddressWindow(uint8_t X0, uint16_t Y0, uint8_t X1, uint8_t Y1)
{
	// Colum address set
	ST7735_WriteCommand(ST7735_CASET);
	uint8_t data[] = { 0x00,    X0 + ST7735_XSTART,     0x00,    X1 + ST7735_XSTART };
	ST7735_WriteData(data, sizeof(data));
	
	// Row address set
	ST7735_WriteCommand(ST7735_RASET);
	data[1] = Y0 + ST7735_YSTART;
	data[3] = Y1 + ST7735_YSTART;
	ST7735_WriteData(data, sizeof(data));
	// Write to RAM
	ST7735_WriteCommand(ST7735_RAMWR);
}


//static void ST7735_SetCursor(uint16_t Xpos, uint16_t Ypos)
//{
//	ST7735_WriteCommand(ST7735_CASET);
//	ST7735_WriteData8(Xpos >> 8);
//	ST7735_WriteData8(Xpos & 0xFF);
//	ST7735_WriteCommand(ST7735_RASET); 
//	ST7735_WriteData8(Ypos >> 8); 
//	ST7735_WriteData8(Ypos & 0XFF);
//}

void ST7735_WriteRAM_Prepare(void)
{
	ST7735_WriteCommand(ST7735_RAMWR);  
}

void ST7735_Init() 
{
    ST7735_Select();
    ST7735_Reset();
    ST7735_ExecuteCommandList(Init_Cmds1);
    ST7735_ExecuteCommandList(Init_Cmds2);
    ST7735_ExecuteCommandList(Init_Cmds3);
    ST7735_Unselect();
}





/************************* *************************High level Functions ****************************************************************/
void ST7735_DisplayON(void)
{
	ST7735_Select();
	ST7735_WriteCommand(ST7735_DISPON);
	ST7735_delay(10);
	ST7735_Unselect();
}

void ST7735_DisplayOFF(void)
{
	ST7735_Select();
	ST7735_WriteCommand(ST7735_DISPOFF);
	ST7735_delay(10);
	ST7735_Unselect();
}

static void ST7735_WriteChar(uint8_t X_Axis, uint8_t Y_Axis, char ch, FontDef font, uint16_t color, uint16_t bgColor)
{
    uint32_t i, b, j;
    ST7735_SetAddressWindow(X_Axis, Y_Axis, (X_Axis+font.width - 1), (Y_Axis + font.height - 1));
    for(i = 0; i < font.height; i++) {
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++) {
            if((b << j) & 0x8000)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ST7735_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = { bgColor >> 8, bgColor & 0xFF };
                ST7735_WriteData(data, sizeof(data));
            }
        }
    }
}


void ST7735_WriteString(uint8_t X_Axis, uint8_t Y_Axis, const char *str, FontDef font, uint16_t color, uint16_t bgColor)
{
    ST7735_Select();
    while(*str) {
        if(X_Axis + font.width >= ST7735_WIDTH) {
            X_Axis = 0;
            Y_Axis += font.height;
            if(Y_Axis + font.height >= ST7735_HEIGHT) {
                break;
            }
            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }
		if(bgColor != 0x0001) ST7735_WriteChar(X_Axis, Y_Axis, *str, font, color, bgColor);
        X_Axis += font.width;
        str++;
    }
    ST7735_Unselect();
}

// Ve 1 diem anh
void ST7735_DrawPixel(uint8_t X_Axis, uint16_t Y_Axis, uint16_t color)
{
    if((X_Axis >= ST7735_WIDTH) || (Y_Axis >= ST7735_HEIGHT)) return;
    
    ST7735_Select();
    ST7735_SetAddressWindow(X_Axis, Y_Axis, (X_Axis + 1), (Y_Axis + 1));
    uint8_t data[] = { (color >> 8), (color & 0xFF) };
    ST7735_WriteData(data, sizeof(data));
    ST7735_Unselect();
}

// Ve duong ngang
void ST7735_HozizontalLine(uint8_t X_Start, uint8_t Y_Start, uint8_t Length, uint16_t color)
{
    if((X_Start >= ST7735_WIDTH) || (Y_Start >= ST7735_HEIGHT)) return;
    
    for(uint8_t i = X_Start; i < Length + X_Start - 1; i++)
    {
        ST7735_DrawPixel(i, Y_Start, color);
    }
}

// Ve chieu doc
void ST7735_VerticalLine(uint8_t X_Start, uint8_t Y_Start, uint8_t Length, uint16_t color)
{
    if((X_Start >= ST7735_WIDTH) || (Y_Start >= ST7735_HEIGHT)) return;
    
    for(uint8_t i = Y_Start; i < Length + Y_Start - 1; i++)
    {
        ST7735_DrawPixel(X_Start, i, color);
    }
}
/*
void ST7735_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t colour)
{
	int16_t F;
	int16_t x;
	int16_t y;
	int16_t dy;
	int16_t dx;
	int16_t dy2;
	int16_t dx2;
	int16_t dy2_minus_dx2;
	int16_t dy2_plus_dx2;

	if (x1 > x2) {
		F = x1;
		x1 = x2;
		x2 = F;

		F = y1;
		y1 = y2;
		y2 = F;
	}

	dy = y2 - y1;
	dx = x2 - x1;
	dy2 = (dy << 1);
	dx2 = (dx << 1);
	dy2_minus_dx2 = dy2 - dx2;
	dy2_plus_dx2 = dy2 + dx2;

	if (dy >= 0) {
		if (dy <= dx) {
			F = dy2 - dx;
			x = x1;
			y = y1;
			while (x <= x2) {
				ST7735_DrawPixel(x,y,colour);
				if (F <= 0) {
					F += dy2;
				
				} else {
					y++;
					F += dy2_minus_dx2;
				}
				x++;
			}
		} else {
			F = dx2 - dy;
			y = y1;
			x = x1;
			while (y <= y2) {
				ST7735_DrawPixel(x, y, colour);
				if (F <= 0) {
					F += dx2;
				} else {
					x++;
					F -= dy2_minus_dx2;
				}
			y++;
			}
		}
	} else {
		if (dx >= -dy) {
			F = -dy2 - dx;
			x = x1;
			y = y1;
			while (x <= x2) {
				ST7735_DrawPixel(x, y, colour);
				if ( F <= 0) {
					F -= dy2;
				} else {
					y--;
					F -= dy2_plus_dx2;
				}
				x++;
			}
		} else {
			F = dx2 + dy;
			y = y1;
			x = x1;
			while (y >= y2) {
				ST7735_DrawPixel(x, y, colour);
				if (F <= 0) {
					F += dx2;
				} else {
					x++;
					F += dy2_plus_dx2;
				}
				y--;
			}
		}
	}
}
   */
void ST7735_Rectangle(uint8_t X_Start, uint8_t Y_Start, uint8_t Width, uint8_t Height, uint16_t Color)
{
    if((X_Start >= ST7735_WIDTH) || (Y_Start >= ST7735_HEIGHT)) return;
    
    ST7735_HozizontalLine(X_Start, Y_Start, Width, Color);
    ST7735_HozizontalLine(X_Start, (Y_Start + Height - 1), Width, Color);
    
    ST7735_VerticalLine(X_Start, Y_Start, Height, Color);
    ST7735_VerticalLine((X_Start + Width - 1), Y_Start, Height, Color);
}

void ST7735_FillRectangle(uint8_t X_Start, uint8_t Y_Start, uint8_t Width, uint8_t Height, uint16_t Color)
{
    if((X_Start >= ST7735_WIDTH) || (Y_Start >= ST7735_HEIGHT)) return;
    
    if((X_Start + Width - 1) >= ST7735_WIDTH)
    {
        Width = ST7735_WIDTH - X_Start;
    }
    if((Y_Start + Height - 1) >= ST7735_HEIGHT)
    {
        Height = ST7735_HEIGHT - Y_Start;
    }
    
    ST7735_Select();
    ST7735_SetAddressWindow(X_Start, Y_Start, (X_Start + Width - 1), (Y_Start + Height - 1));
    
    uint8_t data[] = { (Color >> 8), (Color & 0xFF) };
    for(Y_Start = Height; Y_Start > 0; Y_Start--){
        for(X_Start = Width; X_Start > 0; X_Start --){
            ST7735_WriteData(data, sizeof(data));
        }
    }
    ST7735_Unselect();
}

void ST7735_FillScreen(uint16_t Color)
{
    ST7735_FillRectangle(0, 0, ST7735_WIDTH, ST7735_HEIGHT, Color);
}
        

void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h,const  uint8_t* data) {
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if((x + w - 1) >= ST7735_WIDTH) return;
    if((y + h - 1) >= ST7735_HEIGHT) return;
    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x+w, y+h);
	ST7735_DATA();
	SPI_DMA_Send(SPI1, (uint8_t * )data,2 * w * h);
	while(SPI1->SR & SPI_SR_BSY);
    ST7735_Unselect();
}
        

//void ST7735_Configuration(SPI_TypeDef *pSPI, GPIO_TypeDef *pGPIO, uint16_t pinCS, uint16_t pinRS, uint16_t pinDC)
void ST7735_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  // APB2: 72Mhz => SPI1 = 72/4 = 18MBit/s

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	
	//if(pSPI == SPI1) {
		// (Pin 7: MOSI,  Pin 5: CLK) AF_PP
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		// Pin 6: MISO IN FLOATING
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_Init(GPIOA, &GPIO_InitStructure);


}


	
