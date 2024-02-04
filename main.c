#include "main.h"
#include "Fuzzy_Logic.h"
#include "Delay_Timer.h"
#include "ST7735_TFT_SPI.h"
#include "Loadcell_HX711.h"
#include "KieuGiat.h"

#pragma pack(1)
typedef struct{
	_Bool State_Door;
	_Bool State_Value;
	uint8_t List;
	uint8_t Weight_Max;
	uint8_t Value_Temperature;
	uint16_t Value_Spin;
	float Weight;
}Data_Peripheral;
#pragma pack()


typedef struct{
	uint8_t kieuGiat;
	uint8_t Time_Fuzzy;
	uint8_t nhietDoNuoc;
	uint8_t ListtocDo;
}Data_Washing;


typedef struct{
	uint8_t kieuGiat;
	uint8_t thoiGianConLai;
	uint8_t trangThaiHoatDong;
	float nhietDoTT;
}Data_Task_Cycle_Excution;


uint16_t ADC_Buffer[2];

QueueHandle_t xQueuePeripheral;
QueueHandle_t xQueueWashing;
QueueHandle_t xQueueCycleExcution;
QueueSetHandle_t xQueueReceiveTFT;


TaskHandle_t Task_ReadPeripheral_Handle;
TaskHandle_t Task_Cycle_Excution_Handle;

SemaphoreHandle_t xBinarySemaphoreBuzzer;


 static void Task_TFT(void *pvParameter);
static void Task_Read_Peripheral(void *pvParameter);
static void Task_Cycle_Excution(void *pvParameter);
static void Task_Buzzer(void *pvParameter);

void vienChonCheDo(uint8_t List);

int main(void)
{
	GPIO_Configuration();
	ADC_DMA_Configuration((uint32_t*)ADC_Buffer);
	Delay_Timer_Init();	// TIM4
	PWM_Init();
	ST7735_Configuration(); //SPI1, GPIOA, GPIO_Pin_4, GPIO_Pin_3, GPIO_Pin_2
	SPI_DMA_Configuration(); // SPI1
	ST7735_Init();
	HX711_Init(); //GPIOB, GPIO_Pin_6, GPIOB, GPIO_Pin_7
	
	xTaskCreate(Task_TFT, "Task_TFT", 128, NULL, 2, NULL);
	xTaskCreate(Task_Read_Peripheral, "Task_R", 128, NULL, 1, &Task_ReadPeripheral_Handle);
	xTaskCreate(Task_Cycle_Excution, "Task_C", 128, NULL, 1, &Task_Cycle_Excution_Handle);
	xTaskCreate(Task_Buzzer, "Task_Buzzer",128, NULL, 3, NULL);
	
	xQueuePeripheral      = xQueueCreate(1, sizeof(Data_Peripheral));
	xQueueWashing        = xQueueCreate(1, sizeof(Data_Washing));
	xQueueCycleExcution = xQueueCreate(1, sizeof(Data_Task_Cycle_Excution));
	xQueueReceiveTFT     = xQueueCreateSet(2);	// Queue Length Peripheral + Queue Lenth Cycle Excution
	
	// Associate the semaphore and queue to the queue set handle
	xQueueAddToSet(xQueuePeripheral, xQueueReceiveTFT);
	xQueueAddToSet(xQueueCycleExcution, xQueueReceiveTFT);
	
	xBinarySemaphoreBuzzer = xSemaphoreCreateBinary();
	vTaskStartScheduler();
	while(1){
	}
}


static void Task_Buzzer(void *pvParameter){
	for(;;){
		xSemaphoreTake(xBinarySemaphoreBuzzer, portMAX_DELAY);
		GPIO_SetBits(Port_Buzzer, Pin_Buzzer);
		vTaskDelay(80);
		GPIO_ResetBits(Port_Buzzer, Pin_Buzzer);
		vTaskDelay(80);
	}
}


static void Task_TFT(void *pvParameter){
	ST7735_DrawImage(0,0,160,128,meo_map);
	ST7735_WriteString(25,90, "WASHING MACHINE", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	ST7735_WriteString(45, 110, "FUZZY CAT", Font_7x10, ST7735_WHITE, ST7735_BLACK);
	
	Data_Peripheral Data_Peripheral_Received;
	Data_Task_Cycle_Excution Data_Task_Cycle_Excution_Receive;
	
	//------------------------------------------------ Data Task Periph ---------------------------------------------------------//
	uint8_t u8Old_Value_List,  u8Old_Value_Temperature, u8Old_Value_Weight_Max;
	uint16_t u16Old_Value_Spin;
	float fOld_Value_Weight_Clothers;
	 _Bool Block_First = 0;
	char data_Conv1[4], data_Conv2[4], data_Conv3[4], data_Conv4[4];
	static char  *tenKieuGiat;
	
	//------------------------------------------------ Data Task Cycle Excuton --------------------------------------------//
	uint8_t Old_KieuGiat, Old_ChuTrinhHienTai;

	for( ;; )
	{
		//------------------------------------------------------Receive from task periph-----------------------------------------------------------------------------//
		QueueSetMemberHandle_t Who_Unblocked = xQueueSelectFromSet(xQueueReceiveTFT, portMAX_DELAY);
		if( Who_Unblocked == xQueuePeripheral)
		{
			if(xQueueReceive(xQueuePeripheral, &Data_Peripheral_Received, (TickType_t) 0) == pdPASS)
			{
				if(Data_Peripheral_Received.List != u8Old_Value_List)
				{
					// Xoa man hinh chao mung va chuyen sang hien thi chon menu
					if((Data_Peripheral_Received.List != 0) && Block_First == 0)
					{
						ST7735_FillScreen(ST7735_BLACK);
						ST7735_Rectangle(0, 0, 160, 128, ST7735_WHITE);
						ST7735_VerticalLine(110, 0,128,ST7735_WHITE);
						ST7735_HozizontalLine(110, 42, 54, ST7735_WHITE);
						ST7735_HozizontalLine(110, 84, 54, ST7735_WHITE);
				
						ST7735_WriteString(115, 5, "WEIGHT", Font_7x10, ST7735_GREENYELLOW, ST7735_BLACK);
						ST7735_WriteString(120, 50, "TEMP", Font_7x10, ST7735_RED, ST7735_BLACK);
						ST7735_WriteString(120, 92, "SPIN", Font_7x10, ST7735_BLUE, ST7735_BLACK);
						// Menu Mode
						ST7735_WriteString(7, 8 , "VAI BONG", Font_7x10, ST7735_WHITE, ST7735_BLACK);
						ST7735_WriteString(7,33, "DO HON HOP", Font_7x10, ST7735_WHITE, ST7735_BLACK);
						ST7735_WriteString(7,58, "DO TRAI GIUONG", Font_7x10, ST7735_WHITE, ST7735_BLACK);
						ST7735_WriteString(7,83, "GIAT NHE", Font_7x10, ST7735_WHITE, ST7735_BLACK);
						ST7735_WriteString(7,108, "DO THE THAO", Font_7x10, ST7735_WHITE, ST7735_BLACK);
						Block_First = 1;
					}
				vienChonCheDo(Data_Peripheral_Received.List);
			
					if(Data_Peripheral_Received.List == 6 || Data_Peripheral_Received.List == 7)
					{
						ST7735_FillRectangle(1,1, 109, 126, ST7735_BLACK);
						// Bao loi chua dong cua
						if(Data_Peripheral_Received.State_Door == true)	ST7735_WriteString(10,40, "CHUA DONG CUA", Font_7x10, ST7735_WHITE, ST7735_RED);
						// Bao loi qua can nang
						if(Data_Peripheral_Received.State_Value == true) ST7735_WriteString(10, 60, "QUA CAN NANG", Font_7x10, ST7735_WHITE, ST7735_RED);
					}
				}
				u8Old_Value_List = Data_Peripheral_Received.List;
		
				//----------------------------------------------Hien thi can nang quan ao-------------------------------------------------------//
				if(Data_Peripheral_Received.Weight != fOld_Value_Weight_Clothers && Block_First == 1)
				{
					sprintf(data_Conv1, "%.1f", Data_Peripheral_Received.Weight);
					ST7735_WriteString(111, 22, data_Conv1, Font_7x10, ST7735_GREENYELLOW, ST7735_BLACK);
				}
				fOld_Value_Weight_Clothers = Data_Peripheral_Received.Weight;
		
				//---------------------------------------------Hien thi cai dat trong luong--------------------------------------------------------//
				if(Data_Peripheral_Received.Weight_Max != u8Old_Value_Weight_Max && Block_First == 1)
				{
					sprintf(data_Conv2, "/%dKG", Data_Peripheral_Received.Weight_Max);
					ST7735_WriteString(131,22, data_Conv2, Font_7x10, ST7735_GREENYELLOW, ST7735_BLACK);
				}
				u8Old_Value_Weight_Max = Data_Peripheral_Received.Weight_Max;
		
				//---------------------------------------------Hien thi cai dat nhiet do-----------------------------------------------------------//
				if(Data_Peripheral_Received.Value_Temperature != u8Old_Value_Temperature && Block_First == 1)
				{
					sprintf(data_Conv3, "%d*C", Data_Peripheral_Received.Value_Temperature);
					if(Data_Peripheral_Received.Value_Temperature == 0) {
						data_Conv3[0] = 'N'; data_Conv3[1] = 'O';data_Conv3[2] = 'R'; data_Conv3[3] = 'M';
					}
					ST7735_WriteString(118, 67, data_Conv3, Font_7x10, ST7735_RED, ST7735_BLACK);
				} 
				u8Old_Value_Temperature = Data_Peripheral_Received.Value_Temperature;
		
				//-------------------------------------------Hien thi cai dat toc do vat-----------------------------------------------------------//
				if(Data_Peripheral_Received.Value_Spin != u16Old_Value_Spin && Block_First == 1){
					sprintf(data_Conv4, "%d", Data_Peripheral_Received.Value_Spin);
					ST7735_FillRectangle(123, 107, 30, 10, ST7735_BLACK);
					ST7735_WriteString(123, 107, data_Conv4, Font_7x10, ST7735_BLUE, ST7735_BLACK);
				} 
				u16Old_Value_Spin = Data_Peripheral_Received.Value_Spin;
			}
		}
		//---------------------------------------------------------Receive from task cycle excution-------------------------------------------------------------------------------// 
		else if(Who_Unblocked == xQueueCycleExcution)
		{
			if(xQueueReceive(xQueueCycleExcution, &Data_Task_Cycle_Excution_Receive, (TickType_t)0) == pdPASS)
			{
				if(Data_Task_Cycle_Excution_Receive.kieuGiat != Old_KieuGiat)
				{
					if( Data_Task_Cycle_Excution_Receive.kieuGiat == 1)	tenKieuGiat = "VAI BONG";
					else if(Data_Task_Cycle_Excution_Receive.kieuGiat == 2) tenKieuGiat = "DO HON HOP";
					else if(Data_Task_Cycle_Excution_Receive.kieuGiat == 3) tenKieuGiat = "DO TRAI GIUONG";
					else if(Data_Task_Cycle_Excution_Receive.kieuGiat == 4) tenKieuGiat = "GIAT NHE";
					else if(Data_Task_Cycle_Excution_Receive.kieuGiat == 5) tenKieuGiat = "DO THE THAO"; 
					ST7735_WriteString( 10, 5, tenKieuGiat, Font_7x10, ST7735_ORANGE, ST7735_BLACK);
					ST7735_Rectangle(0, 21, 111, 30, ST7735_WHITE);
				}
				Old_KieuGiat = Data_Task_Cycle_Excution_Receive.kieuGiat;
			
				sprintf(data_Conv4, "0:%d", Data_Task_Cycle_Excution_Receive.thoiGianConLai);
				ST7735_WriteString( 40, 30, data_Conv4, Font_7x10, ST7735_ORANGE, ST7735_BLACK);
				sprintf(data_Conv3, "%.1f", Data_Task_Cycle_Excution_Receive.nhietDoTT);
				ST7735_WriteString(40, 60, data_Conv3, Font_7x10, ST7735_ORANGE, ST7735_BLACK);
				if(Data_Task_Cycle_Excution_Receive.trangThaiHoatDong != Old_ChuTrinhHienTai)
				{
					if(Data_Task_Cycle_Excution_Receive.trangThaiHoatDong == 1 || Data_Task_Cycle_Excution_Receive.trangThaiHoatDong == 5) tenKieuGiat = "CAP NUOC ";
					else if(Data_Task_Cycle_Excution_Receive.trangThaiHoatDong == 2 || Data_Task_Cycle_Excution_Receive.trangThaiHoatDong == 6) tenKieuGiat = "DUN NUOC ";
					else if(Data_Task_Cycle_Excution_Receive.trangThaiHoatDong == 3) tenKieuGiat = "DANG GIAT";
					else if(Data_Task_Cycle_Excution_Receive.trangThaiHoatDong == 4 || Data_Task_Cycle_Excution_Receive.trangThaiHoatDong == 8 || Data_Task_Cycle_Excution_Receive.trangThaiHoatDong == 10) tenKieuGiat = "XA NUOC  ";
					else if(Data_Task_Cycle_Excution_Receive.trangThaiHoatDong == 7) tenKieuGiat = "DANG GIU ";
					else if(Data_Task_Cycle_Excution_Receive.trangThaiHoatDong == 9) tenKieuGiat = "DANG VAT ";
					else if(Data_Task_Cycle_Excution_Receive.trangThaiHoatDong == 11) tenKieuGiat = "GIAT XONG";
					ST7735_WriteString(25, 80, tenKieuGiat, Font_7x10, ST7735_ORANGE, ST7735_BLACK);
				}
			Old_ChuTrinhHienTai = Data_Task_Cycle_Excution_Receive.trangThaiHoatDong;
			}
		}
	}
}





static void Task_Read_Peripheral(void *pvParameter){
	 _Bool bOld_Status_Temp, bOld_Status_Spin, bOld_Status_Start, bCurrentStateCLK, bLastStateCLK  = 1;
	 uint8_t u8Count = 0, u8Time_Fuzzy;
	 float fActualWeightClothers = 0;
	 Data_Peripheral Data_Peripheral_Send;
	 Data_Washing Data_Washing_Send;
	pKieuGiat = &VaiBong;
	TickType_t lastDelay = xTaskGetTickCount();
	
	for(;;){
		//-----------------------------------------------------------Read Loadcell--------------------------------------------------------------------//
		if(xTaskGetTickCount() - lastDelay > (TickType_t)250)
		{
			fActualWeightClothers = ((HX711_Value() - 7921200) / 342900.0f);   // 342.90: 1g 3429.0: 10g    342900f: 100g 342900: 1kg
			if(fActualWeightClothers < 0.0f) fActualWeightClothers = 0.0f;
			Data_Peripheral_Send.Weight = fActualWeightClothers;
			lastDelay = xTaskGetTickCount();
		}
		
		
		//---------------------------------------------------------Read Rotary Encoder---------------------------------------------------------------//
		bCurrentStateCLK = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);
		if(bCurrentStateCLK != bLastStateCLK && bCurrentStateCLK == 1){
			xSemaphoreGive(xBinarySemaphoreBuzzer);
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) != bCurrentStateCLK){
				u8Count++;
			} else {
				u8Count--;
			}
			if(u8Count > 5) u8Count = 1;
			if(u8Count < 1) u8Count = 5;

			if(u8Count == 1) pKieuGiat = &VaiBong; 
			else if(u8Count == 2) pKieuGiat = &DoHonHop;
			else if(u8Count == 3) pKieuGiat = &DoTraiGiuong;
			else if(u8Count == 4) pKieuGiat = &GiatNhe;
			else if(u8Count == 5) pKieuGiat = &DoTheThao;
			
			Data_Peripheral_Send.Value_Temperature = pKieuGiat->Set_Temperature;
			Data_Peripheral_Send.Weight_Max = pKieuGiat->Weight_Max;
			Data_Peripheral_Send.Value_Spin = pKieuGiat->Set_Spin;
			Data_Peripheral_Send.List = u8Count;

		} bLastStateCLK = bCurrentStateCLK;
		
		//----------------------------------------------------------Read Touch Temperature--------------------------------------------------------------//
		if(Touch_Temp){
			if(Touch_Temp && !bOld_Status_Temp){
				xSemaphoreGive(xBinarySemaphoreBuzzer);
				pKieuGiat->Switch_Temp++;
				if(pKieuGiat->Switch_Temp > 5) pKieuGiat->Switch_Temp = 0;
				switch(pKieuGiat->Switch_Temp){
					case 1: { Data_Peripheral_Send.Value_Temperature = pKieuGiat->Set_Temperature = 20; break; }
					case 2: { Data_Peripheral_Send.Value_Temperature = pKieuGiat->Set_Temperature = 30; break; }
					case 3: { Data_Peripheral_Send.Value_Temperature = pKieuGiat->Set_Temperature = 40; break; }
					case 4: { Data_Peripheral_Send.Value_Temperature = pKieuGiat->Set_Temperature = 60; break; }
					case 5: { Data_Peripheral_Send.Value_Temperature = pKieuGiat->Set_Temperature = 95; break; }
					default:{ Data_Peripheral_Send.Value_Temperature = pKieuGiat->Set_Temperature = 0;  break; }
				}
			}
		} bOld_Status_Temp = Touch_Temp;
	
		//------------------------------------------------------------Read Touch Spin----------------------------------------------------------------//
		if(Touch_Spin){
			if(Touch_Spin && ! bOld_Status_Spin){
				xSemaphoreGive(xBinarySemaphoreBuzzer);
				pKieuGiat->Switch_Spin++;
				if(pKieuGiat->Switch_Spin > 5) pKieuGiat->Switch_Spin = 0;
				switch(pKieuGiat->Switch_Spin){
					case 1: { Data_Peripheral_Send.Value_Spin = pKieuGiat->Set_Spin = 400;   break; }
					case 2: { Data_Peripheral_Send.Value_Spin = pKieuGiat->Set_Spin = 800;   break; }
					case 3: { Data_Peripheral_Send.Value_Spin = pKieuGiat->Set_Spin = 1000; break; }
					case 4: { Data_Peripheral_Send.Value_Spin = pKieuGiat->Set_Spin = 1200; break; }
					case 5: { Data_Peripheral_Send.Value_Spin = pKieuGiat->Set_Spin = 1400; break; }
					default:{ Data_Peripheral_Send.Value_Spin = pKieuGiat->Set_Spin = 0;	  break; }
				}
			}
		} bOld_Status_Spin = Touch_Spin;
		
		//--------------------------------------------------------Read Touch Start----------------------------------------------------------------//
		if(Touch_Start && u8Count != 0){
			if(Touch_Start  && !bOld_Status_Start){
				xSemaphoreGive(xBinarySemaphoreBuzzer);
				// Clear display
				Data_Peripheral_Send.List = 6;				
				// Check Door Limit Switch and weight
				if(Door_Limit_Switch == 0 && fActualWeightClothers <= (float)pKieuGiat->Weight_Max) {
					// do thing
					Data_Peripheral_Send.List = 7;
					Data_Peripheral_Send.State_Door = false;
					 Data_Peripheral_Send.State_Value = false;
					
					// Khoi tao bo sinh so ngau nhien va tinh toan thoi gian qua Fuzzy Logic
					srand(SysTick->VAL);
					Fuzzy_Logic_Input(fActualWeightClothers + 3, rand() % 100);
					fis_evaluate();
					u8Time_Fuzzy = Fuzzy_Logic_Output();
					
					Data_Washing_Send.nhietDoNuoc = pKieuGiat->Set_Temperature;
					Data_Washing_Send.ListtocDo = pKieuGiat->Switch_Spin;
					Data_Washing_Send.Time_Fuzzy = u8Time_Fuzzy;
					Data_Washing_Send.kieuGiat = u8Count;
					xQueueSend(xQueuePeripheral, &Data_Peripheral_Send, (TickType_t)0);
					xQueueSend(xQueueWashing, &Data_Washing_Send, (TickType_t)0);
					vTaskDelay(20);
					vTaskResume(Task_Cycle_Excution_Handle);
					vTaskSuspend(Task_ReadPeripheral_Handle);
				} else {
					if(Door_Limit_Switch) Data_Peripheral_Send.State_Door = true;
					if(fActualWeightClothers >= (float)pKieuGiat->Weight_Max) Data_Peripheral_Send.State_Value = true;
				}
			}
		} bOld_Status_Start = Touch_Start;
			
		xQueueSend(xQueuePeripheral, &Data_Peripheral_Send, (TickType_t)0);
		
	}
}	


static void Task_Cycle_Excution(void *pvParameter){
	vTaskSuspend(NULL);
	Data_Washing Data_Washing_Receive;
	Data_Task_Cycle_Excution Data_Task_CycleExcution_Send;
	
	float fR_ohms, fActualTemperature;	// Bien nhiet do
	
	static uint8_t u8tongThoiGian, u8thoiGianGiat, u8thoiGianXa, u8thoiGianVat, demThoiGian, thoiGianNghi = 3;
	uint8_t u8SetWaterTemperature, u8SetSpin, u8ValuePWM, u8congViecHienTai = 1;
	static _Bool RightOrLeft = 0;
	TickType_t lastDelay = xTaskGetTickCount();
	for( ;; ){
		//--------------------------------------------- Nhan du lieu tu Task Periph-----------------------------------------------------------------------//
		// Parameter: Kieu giat, Time Fuzzy, Nhiet do nuoc, toc do quay 
		if(xQueueReceive(xQueueWashing, &Data_Washing_Receive, (TickType_t)0) == pdPASS)
		{
			Data_Task_CycleExcution_Send.kieuGiat = Data_Washing_Receive.kieuGiat;
			u8tongThoiGian = Data_Washing_Receive.Time_Fuzzy;
			u8SetWaterTemperature = Data_Washing_Receive.nhietDoNuoc;
			u8SetSpin = Data_Washing_Receive.ListtocDo;
			
			Data_Task_CycleExcution_Send.thoiGianConLai = u8tongThoiGian;
			u8thoiGianGiat = (uint8_t)(u8tongThoiGian * 0.6f);
			u8thoiGianXa   = (uint8_t)(u8tongThoiGian * 0.3f);
			u8thoiGianVat  = u8tongThoiGian - (u8thoiGianGiat + u8thoiGianXa);
		}
		
		//----------------------------------------------------- Doc nhiet do nuoc -----------------------------------------------------------------------------//
		if(xTaskGetTickCount() - lastDelay > 250)
		{
			fR_ohms = 10000 / (4095.0f / (float)ADC_Buffer[0] - 1.0f); 		// 10000: seriesesistor
			fActualTemperature = 1 / ((logf(fR_ohms  / 10000.0f) / 3950.0f) + (1.0f / (25.0f + 273.15f))) - 273.15f; 
			Data_Task_CycleExcution_Send.nhietDoTT = fActualTemperature;
			lastDelay = xTaskGetTickCount();
		}
		
		//------------------------------------------------Thuc hien chu trinh  ------------------------------------------------------------------------//
		if(u8congViecHienTai == 1)// Kiem tra muc nuoc
		{  
			if(ADC_Buffer[1] > 2000){
				u8congViecHienTai = 2;
				vTaskDelay(1000);
			}
		} 
		else if (u8congViecHienTai == 2) // Kiem tra nhiet do nuoc
		{	
			// Kiem tra nhiet do nuoc
			if(fActualTemperature <= u8SetWaterTemperature)
			{
				GPIO_ResetBits(Port_HeatBar, Pin_HeatBar);
			} else { 
				GPIO_SetBits(Port_HeatBar, Pin_HeatBar);
				u8congViecHienTai = 3;
				vTaskDelay(1000);
			}
		} 
		else if (u8congViecHienTai == 3) // Thuc hien chu trinh giat
		{
			// 1,2: quay phai,  3: Nghi,   4,5: Quay trai,   6:Nghi ..............
			demThoiGian = 0, thoiGianNghi = 3;
			RightOrLeft = 0;
			for(demThoiGian = 1; demThoiGian <= u8thoiGianGiat; demThoiGian++)
			{
				if(demThoiGian == u8thoiGianGiat)	// Khi chay du thoi gian giat cho dung dong co
				{
					GPIOA->ODR &= ~(1 << 9);
					GPIOB->ODR &= ~(1 << 15);
					TIM1->CCR1 = 90;
					u8congViecHienTai = 4;
				}
				else if(demThoiGian < thoiGianNghi) // Cho dong co quay trai hoac quay phai
				{	
					if(RightOrLeft == 0)		
					{																			
						GPIOA->ODR |= (1 << 9);
						GPIOB->ODR &= ~(1 << 15);
						TIM1->CCR1 = 36;
					} else {																	
						GPIOA->ODR &= ~(1 << 9);
						GPIOB->ODR |= (1 << 15);
						TIM1->CCR1 = 36; 
					}
				} 
				else if(demThoiGian == thoiGianNghi) // Cho motor dung truoc khi dao chieu
				{
					GPIOA->ODR &= ~(1 << 9);
					GPIOB->ODR &= ~(1 << 15);
					TIM1->CCR1 = 90;
					RightOrLeft = !RightOrLeft;
					thoiGianNghi = demThoiGian + 3;
				}
				vTaskDelay(1000);
				u8tongThoiGian--;
				Data_Task_CycleExcution_Send.thoiGianConLai = u8tongThoiGian;
				Data_Task_CycleExcution_Send.trangThaiHoatDong = u8congViecHienTai;
				xQueueSend(xQueueCycleExcution, &Data_Task_CycleExcution_Send, (TickType_t)0);
				}
		}
		else if(u8congViecHienTai == 4)	// Bom nuoc thai
		{
			if(ADC_Buffer[1] > 2000) {
				GPIO_ResetBits(Port_Pump, Pin_Pump);	// Bat may bom nuoc
			} else if(ADC_Buffer[1] < 100) {
				GPIO_SetBits(Port_Pump, Pin_Pump);	// Tat may bom
				u8congViecHienTai = 5;
				vTaskDelay(1000);
			}
		}
		else if(u8congViecHienTai == 5)	// Kiem tra muc nuoc
		{
			if(ADC_Buffer[1] > 2000){
				u8congViecHienTai = 6;
				vTaskDelay(1000);
			}
		}
		else if(u8congViecHienTai == 6)	// Kiem tra nhiet do nuoc
		{
			if(fActualTemperature <= u8SetWaterTemperature){
				GPIO_ResetBits(Port_HeatBar, Pin_HeatBar);
			} else {
				GPIO_SetBits(Port_HeatBar, Pin_HeatBar);
				u8congViecHienTai = 7;
				vTaskDelay(1000);
			}
		}
		else if(u8congViecHienTai == 7)	// Thuc hien chu trinh xa
		{
			// 1,2: quay phai,  3: Nghi,   4,5: Quay trai,   6:Nghi ..............
			demThoiGian = 0, thoiGianNghi = 3;
			RightOrLeft = 0;
			for(demThoiGian = 1; demThoiGian <= u8thoiGianXa; demThoiGian++)
			{
				if(demThoiGian == u8thoiGianXa)	// Khi chay du thoi gian giat cho dung dong co
				{
					GPIOA->ODR &= ~(1 << 9);
					GPIOB->ODR &= ~(1 << 15);
					TIM1->CCR1 = 90;
					u8congViecHienTai = 8;
				}
				else if(demThoiGian < thoiGianNghi) // Cho dong co quay trai hoac quay phai
				{	
					if(RightOrLeft == 0)		
					{																			
						GPIOA->ODR |= (1 << 9);
						GPIOB->ODR &= ~(1 << 15);
						TIM1->CCR1 = 25;
					} else {																	
						GPIOA->ODR &= ~(1 << 9);
						GPIOB->ODR |= (1 << 15);
						TIM1->CCR1 = 25;
					}
				} 
				else if(demThoiGian == thoiGianNghi) // Cho motor dung truoc khi dao chieu
				{
						GPIOA->ODR &= ~(1 << 9);
						GPIOB->ODR &= ~(1 << 15);
						TIM1->CCR1 = 90;
						RightOrLeft = !RightOrLeft;
						thoiGianNghi = demThoiGian + 3;
				}
				vTaskDelay(1000);
				u8tongThoiGian--;
				Data_Task_CycleExcution_Send.thoiGianConLai = u8tongThoiGian;
				Data_Task_CycleExcution_Send.trangThaiHoatDong = u8congViecHienTai;
				xQueueSend(xQueueCycleExcution, &Data_Task_CycleExcution_Send, (TickType_t)0);
				}
		}
		else if(u8congViecHienTai == 8)	// Bom nuoc thai
		{
			if(ADC_Buffer[1] > 2000) {
				GPIO_ResetBits(Port_Pump, Pin_Pump);	// Bat may bom nuoc
			} else if(ADC_Buffer[1] < 100) {
				GPIO_SetBits(Port_Pump, Pin_Pump);	// Tat may bom
				u8congViecHienTai = 9;
				vTaskDelay(1000);
			}
		}
		else if(u8congViecHienTai == 9) // Thuc hien chu trinh vat
		{
			switch(u8SetSpin){
				case 0: {u8ValuePWM = 90; 	break;}
				case 1: {u8ValuePWM = 16; 	break;}
				case 2: {u8ValuePWM = 12; 	break;}
				case 3: {u8ValuePWM = 8; 	break;}
				case 4: {u8ValuePWM = 4; 	break;}
				case 5: {u8ValuePWM = 0; break;}
				default:{ 							break; }
			}
				GPIOA->ODR |= (1 << 9);
				GPIOB->ODR &= ~(1 << 15);
				TIM1->CCR1 = u8ValuePWM;
			while(u8thoiGianVat){
				vTaskDelay(1000);
				u8tongThoiGian--;
				u8thoiGianVat--;
				Data_Task_CycleExcution_Send.thoiGianConLai = u8tongThoiGian;
				Data_Task_CycleExcution_Send.trangThaiHoatDong = u8congViecHienTai;
				xQueueSend(xQueueCycleExcution, &Data_Task_CycleExcution_Send, (TickType_t)0);
			}
			GPIOA->ODR &= ~(1 << 9);
			GPIOB->ODR &= ~(1 << 15);
			TIM1->CCR1 = 0;
			u8congViecHienTai = 10;
			vTaskDelay(2000);
		}
		else if(u8congViecHienTai == 10)	// Bom nuoc thai
		{
			if(ADC_Buffer[1] > 2000) {
				GPIO_ResetBits(Port_Pump, Pin_Pump);	// Bat may bom nuoc
			} else if(ADC_Buffer[1] < 100) {
				GPIO_SetBits(Port_Pump, Pin_Pump);	// Tat may bom
				u8congViecHienTai = 11;
				vTaskDelay(1000);
			}
		}
		else if(u8congViecHienTai == 11)		// Ket thuc chu trinh giat
		{
			u8congViecHienTai = 12;
			vTaskDelay(3000);
		}
		else if(u8congViecHienTai ==12)
		{
			xSemaphoreGive(xBinarySemaphoreBuzzer);
			GPIO_SetBits(Port_TurnOffMachine, Pin_TurnOffMachine);
		}
	Data_Task_CycleExcution_Send.trangThaiHoatDong = u8congViecHienTai;
	xQueueSend(xQueueCycleExcution, &Data_Task_CycleExcution_Send, (TickType_t)0);
	}
}

void vienChonCheDo(uint8_t List){
	if(List == 1){
		ST7735_Rectangle(5, 5, 102, 18, ST7735_ORANGE); // chi vat
		ST7735_Rectangle(5, 30, 102, 18, ST7735_BLACK); // 14 phut
		ST7735_Rectangle(5, 105, 102, 18, ST7735_BLACK);	// Do the thao
	} else if(List == 2){
		ST7735_Rectangle(5, 5, 102, 18, ST7735_BLACK); // chi vat
		ST7735_Rectangle(5, 30, 102, 18, ST7735_ORANGE); // 14 phut
		ST7735_Rectangle(5, 55, 102, 18, ST7735_BLACK);	// Fuzzy logic
	} else if (List == 3){
		ST7735_Rectangle(5, 30, 102, 18, ST7735_BLACK); // 14 phut
		ST7735_Rectangle(5, 55, 102, 18, ST7735_ORANGE); // Fuzzy logic
		ST7735_Rectangle(5, 80, 102, 18, ST7735_BLACK);	// Do hon hop
	} else if(List == 4){
		ST7735_Rectangle(5, 55, 102, 18, ST7735_BLACK); // Fuzzy logic
		ST7735_Rectangle(5, 80, 102, 18, ST7735_ORANGE); // Do hon hop
		ST7735_Rectangle(5, 105, 102, 18, ST7735_BLACK);	// Do the thao
	} else if(List == 5){
		ST7735_Rectangle(5, 80, 102, 18, ST7735_BLACK); // Do hon hop
		ST7735_Rectangle(5, 105, 102, 18, ST7735_ORANGE); // Do the thao
		ST7735_Rectangle(5, 5, 102, 18, ST7735_BLACK);	// chi giat
	}
}
