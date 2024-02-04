#ifndef _ST7735_TFT_SPI_H_
#define _ST7735_TFT_SPI_H_

#include <stm32f10x.h>
#include "SPI.h"
#include "Delay_Timer.h"
#include <stdbool.h>
#include "font.h"
	
#define ST7735_SPI_PORT SPI1
#define DELAY	0x80


#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_ML  0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04




/// AliExpress/eBay 1.8" display, default orientation
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY)
*/

// AliExpress/eBay 1.8" display, rotate right
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV)
*/

// AliExpress/eBay 1.8" display, rotate left
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV)
*/

// AliExpress/eBay 1.8" display, upside down
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (0)
*/

// WaveShare ST7735S-based 1.8" display, default orientation
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB)
*/
// WaveShare ST7735S-based 1.8" display, rotate right
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#define ST7735_XSTART 1
#define ST7735_YSTART 2
#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB)
*/

// WaveShare ST7735S-based 1.8" display, rotate left

#define ST7735_IS_160X128 1
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 128
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB)


// WaveShare ST7735S-based 1.8" display, upside down
/*
#define ST7735_IS_160X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 2
#define ST7735_YSTART 1
#define ST7735_ROTATION (ST7735_MADCTL_RGB)
*/

// 1.44" display, default orientation
//#define ST7735_IS_128X128 1
//#define ST7735_WIDTH  128
//#define ST7735_HEIGHT 128
//#define ST7735_XSTART 2
//#define ST7735_YSTART 3
//#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR)

// 1.44" display, rotate right
/*
#define ST7735_IS_128X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 128
#define ST7735_XSTART 3
#define ST7735_YSTART 2
#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)
*/

// 1.44" display, rotate left
/*
#define ST7735_IS_128X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 128
#define ST7735_XSTART 1
#define ST7735_YSTART 2
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)
*/

// 1.44" display, upside down
/*
#define ST7735_IS_128X128 1
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 128
#define ST7735_XSTART 2
#define ST7735_YSTART 1
#define ST7735_ROTATION (ST7735_MADCTL_BGR)
*/

// mini 160x80 display (it's unlikely you want the default orientation)
/*
#define ST7735_IS_160X80 1
#define ST7735_XSTART 26
#define ST7735_YSTART 1
#define ST7735_WIDTH  80
#define ST7735_HEIGHT 160 
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR)
*/

// mini 160x80, rotate left
/*
#define ST7735_IS_160X80 1
#define ST7735_XSTART 1
#define ST7735_YSTART 26
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 80
#define ST7735_ROTATION (ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)
*/

// mini 160x80, rotate right 
/*
#define ST7735_IS_160X80 1
#define ST7735_XSTART 1
#define ST7735_YSTART 26
#define ST7735_WIDTH  160
#define ST7735_HEIGHT 80
#define ST7735_ROTATION (ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR)
*/




/**************************************************************/
/*							Command define								*/
#define ST7735_NOP				0x00
#define ST7735_SWRESET 	 	0x01					// Software reset
#define ST7735_RDDID   			0x04
#define ST7735_RDDST   			0x09

#define ST7735_SLPIN   			0x10
#define ST7735_SLPOUT  			0x11					// Out of sleep mode
#define ST7735_PTLON   			0x12
#define ST7735_NORON   		0x13

#define ST7735_INVOFF 			0x20				// Don't invert display
#define ST7735_INVON   			0x21
#define ST7735_DISPOFF 			0x28
#define ST7735_DISPON  			0x29
#define ST7735_CASET   			0x2A
#define ST7735_RASET   			0x2B
#define ST7735_RAMWR   		0x2C
#define ST7735_RAMRD   		0x2E
	
#define ST7735_PTLAR   			0x30
#define ST7735_COLMOD 		0x3A					// set color mode
#define ST7735_MADCTL  		0x36					// Memory access control

#define ST7735_FRMCTR1 		0xB1					// Frame rate ctrl  1
#define ST7735_FRMCTR2 		0xB2					// Frame rate ctrl  2
#define ST7735_FRMCTR3 		0xB3					// Frame rate ctrl  3
#define ST7735_INVCTR  			0xB4					// Display inversion ctrl
#define ST7735_DISSET5 			0xB6

#define ST7735_PWCTR1  		0xC0					// Power control 1
#define ST7735_PWCTR2  		0xC1					//	Power control 2
#define ST7735_PWCTR3  		0xC2					// Power control 3
#define ST7735_PWCTR4  		0xC3					// Power control 4
#define ST7735_PWCTR5  		0xC4					// Power control 5
#define ST7735_VMCTR1  		0xC5

#define ST7735_RDID1   			0xDA
#define ST7735_RDID2   			0xDB
#define ST7735_RDID3   			0xDC
#define ST7735_RDID4   			0xDD

#define ST7735_GMCTRP1 		0xE0
#define ST7735_GMCTRN1		0xE1

#define ST7735_PWCTR6  		0xFC




/**************************************************************************/
/*									Color define											*/
#define ST7735_BLACK       					0x0000      
#define ST7735_NAVY        					0x000F     
#define ST7735_DARKGREEN   				0x03E0    
#define ST7735_DARKCYAN    				0x03EF     
#define ST7735_MAROON      				0x7800      
#define ST7735_PURPLE      					0x780F      
#define ST7735_OLIVE       					0x7BE0      
#define ST7735_LIGHTGREY   				0xC618      
#define ST7735_DARKGREY    				0x7BEF      
#define ST7735_BLUE        					0x001F      
#define ST7735_GREEN       					0x07E0      
#define ST7735_CYAN        					0x07FF      
#define ST7735_RED         					0xF800      
#define ST7735_MAGENTA     				0xF81F      
#define ST7735_YELLOW      					0xFFE0     
#define ST7735_WHITE       					0xFFFF      
#define ST7735_ORANGE      				0xFDA0      
#define ST7735_GREENYELLOW 			0xB7E0     
#define ST7735_PINK        					0xFC9F
#define ST7735_NULL							0x0001
#define ST7735_COLOR565(R,B,G)	(((R & 0xF8) << 8) | ((B & 0xFC) << 3) | (( B & 0xF8) >>  3))



/***************************************************************************/
/*	Tap lenh dieu khien: CMD, Delay or num of arg, time delay or arg */

static const uint8_t
  Init_Cmds1[] = {            // Init for 7735R, part 1 (red or green tab)
    15,                       // 15 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
      150,                    //     150 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      255,                    //     500 ms delay
    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                   //     No inversion
    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      0xA2,										//
      0x02,                   //     -4.6V
      0x84,                   //     AUTO mode
    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      0x0A,                   //     Opamp current small
      0x00,                   //     Boost frequency
    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A,  									//
    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,							//
    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      0x0E,										//
    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
    ST7735_ROTATION,        	//     row addr/col addr, bottom to top refresh
    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05 },                 //     16-bit color

#if (defined(ST7735_IS_128X128) || defined(ST7735_IS_160X128))
  Init_Cmds2[] = {            // Init for 7735R, part 2 (1.44" display)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F },           //     XEND = 127
#endif // ST7735_IS_128X128

#ifdef ST7735_IS_160X80
  Init_Cmds2[] = {            // Init for 7735S, part 2 (160x80 display)
    3,                        //  3 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x4F,             //     XEND = 79
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x9F ,            //     XEND = 159
    ST7735_INVON, 0 },        //  3: Invert colors
#endif

  Init_Cmds3[] = {            // Init for 7735R, part 3 (red or green tab)
    4,                        //  4 commands in list:
    ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
      100 };                  //     100 ms delay                					 //     100 ms delay
	
	  
/**********************************************************************************************************/
/*														Function define															 */
//static void ST7735_Reset(void);
//static void ST7735_WriteData(uint8_t* buff, uint8_t buff_size);
//static void ST7735_WriteData8(uint8_t data);
//static void ST7735_WriteData16(uint16_t data);
	  
//void ST7735_Configuration(SPI_TypeDef *pSPI, GPIO_TypeDef *pGPIO, uint16_t pinCS, uint16_t pinRS, uint16_t pinDC);
void ST7735_Configuration(void);
void ST7735_Init(void);
//static void ST7735_SetAddressWindow(uint8_t X0, uint16_t Y0, uint8_t X1, uint8_t Y1);
//static void ST7735_SetCursor(uint16_t Xpos, uint16_t Ypos);
//static void ST7735_WriteRAM_Prepare(void);
// void ST7735_DisplayON(void);
//void ST7735_DisplayOFF(void);
	  
/************************************************ Viet 1 ky tu*******************************************/
// static void ST7735_WriteChar(uint8_t X_Axis, uint8_t Y_Axis, char ch, FontDef font, uint16_t color, uint16_t bgColor);

	
/************************************************ Viet 1 chuoi ky tu*******************************************/
void ST7735_WriteString(uint8_t X_Axis, uint8_t Y_Axis, const char *str, FontDef font, uint16_t color, uint16_t bgColor);


/************************************************ In 1 diem anh *******************************************/
void ST7735_DrawPixel(uint8_t X_Axis, uint16_t Y_Axis, uint16_t color);


/************************************************ In 1 duong thang *******************************************/
void ST7735_HozizontalLine(uint8_t X_Start, uint8_t Y_Start, uint8_t Length, uint16_t color);


/************************************************ In 1 duong ngang *******************************************/
void ST7735_VerticalLine(uint8_t X_Start, uint8_t Y_Start, uint8_t Length, uint16_t color);


/*********************************************** ve bieu do *******************************************/
void ST7735_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t colour);


/************************************************ Ve hinh rong *******************************************/
void ST7735_Rectangle(uint8_t X_Start, uint8_t Y_Start, uint8_t Width, uint8_t Height, uint16_t Color);


/************************************************ Ve hinh dac *******************************************/
void ST7735_FillRectangle(uint8_t X_Start, uint8_t Y_Start, uint8_t Width, uint8_t Height, uint16_t Color);


/************************************************ Ve day man hinh *******************************************/
void ST7735_FillScreen(uint16_t Color);


/************************************************ In 1 anh *******************************************/
void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h,const  uint8_t* data);
#endif
