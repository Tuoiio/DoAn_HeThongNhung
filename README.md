
•	Microcontroller: STM32F103C8

•	Operating System: FreeRTOS

•	Hardware: LCD TFT 1.8inch, Rotary Encoder, Touch Sensor, Loadcell, Sensor Water, Module Thernal Sensor Temperature, Module Relay, Module Drive L298,  Limit Switch, Buzzer.
•	Peripherals: PWM, I/O, ADC, DMA 

•	Protocol: SPI, 2 Wire

•	Technical Skill: C

•	Enviroment development: KeilC, Matlab

•	Description: There are 4 main tasks: Screen task, peripheral reading task, buzzer task, motor control task.
               The rotary encoder is used to select the operating mode. There are 3 touch sensors used to set water temperature, spin speed and confirm running cycles.
               Display selected parameters via LCD TFT screen (using SPI DMA). When you press to confirm the running cycle but the weight is too high or the limit
               switch has not been pressed, an error will be reported. If successful, the Fuzzy Logic algorithm will base on the soiling parameters (random number)
               and the weight of the clothes to determine the time to wash.
               The motor control task will continuously reverse the motor direction until the end of the cycle. The last cycle is the spin cycle with set parameters.
               Finally, it will announce the end of the washing cycle.
![image](https://github.com/Tuoiio/DoAn_HeThongNhung/assets/158831746/0e667822-ec5f-45c2-a6b0-edacf72d87b5)
Màn hình chính
![image](https://github.com/Tuoiio/DoAn_HeThongNhung/assets/158831746/b2761b0f-11b5-44c2-b428-2096686d037f)
Màn hình chọn chế độ

https://www.youtube.com/watch?v=AId59z1tQlM
