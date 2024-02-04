#include "Loadcell_HX711.h"

#if  (_HX711_USE_FREERTOS == 1)
#define HX711_Delay(x)	vTaskDelay(x)
#else 
#define HX711_Delay(x)	Delay_SysTick(x)
#endif



void HX711_Delay_us(void){
	uint32_t delay = _HX711_DELAY_US_LOOP;
	while(delay > 0){
		delay--;
		__nop(); __nop(); __nop(); __nop(); 
	}
}

void HX711_Init(void){

	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

int32_t HX711_Value(void){
	uint32_t data = 0;
	uint8_t gain = 1; 			// if gain = 1 channel A, gain factor 128 //  if gain = 3 channel A, gain factor 64
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == Bit_SET);
	
	for(uint8_t i = 0; i < 24; i++){
		GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_RESET);
		data = data << 1;
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == Bit_SET)
			data++;
	}
  data = data ^ 0x800000; 
	for(uint8_t i = 0; i < gain; i++){
	GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET);   
	GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_RESET);
	}
  return data;
}
/*
//#############################################################################################
int32_t HX711_Value_Average( uint16_t sample)
{
  int64_t  ave = 0;
  for(uint8_t i=0 ; i < sample ; i++) {
    ave += HX711_Value();
  }
  int32_t answer = (int32_t)(ave / sample);

  return answer;
}


void HX711_Tare(uint16_t sample)
{

  int64_t  ave = 0;
  for(uint8_t i=0 ; i<sample ; i++){
    ave += HX711_Value();
  }
  HX711.offset = (int32_t)(ave / sample);

}


void HX711_Calibration( int32_t value_noload, int32_t value_load, float scale){

  HX711.offset = value_noload;
  HX711.coef = (value_load - value_noload) / scale;  

}


float HX711_Weight(uint16_t sample)
{

  int64_t  ave = 0;
  for(uint16_t i=0 ; i < sample ; i++) {
    ave += HX711_Value();
  }
  
  int32_t data = (int32_t)(ave / sample);
  float answer =  (data - HX711.offset) / HX711.coef;

  return answer;
}



void HX711_coef_set(float coef){
  HX711.coef = coef;  
}


float HX711_coef_get(void)
{
  return HX711.coef;  
}
*/


