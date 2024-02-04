#ifndef _LC_HX711_H_
#define _LC_HX711_H_

#define _HX711_USE_FREERTOS 		0
#define _HX711_DELAY_US_LOOP		4

#include "stm32f10x.h"
/*
typedef struct {
	GPIO_TypeDef *CLK_PORT;
	GPIO_TypeDef *DAT_PORT;
	uint16_t CLK_Pin;
	uint16_t DAT_Pin;
	int32_t offset;
	float coef;
}HX711_Handle;
*/
void 	  HX711_Init(void);
int32_t HX711_Value(void);
int32_t HX711_Value_Average(uint16_t sample);

void HX711_coef_set(float coef);
float HX711_coef_get(void);
void HX711_Calibration(int32_t value_noload, int32_t value_load, float scale);
void HX711_Tare(uint16_t sample);
float HX711_Weight(uint16_t sample);

#endif
