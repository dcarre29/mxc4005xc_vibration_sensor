/*
Library to count milliseconds since start up, using Timer4 (8 bit) of STM8S MCUs

By ETPH 20160618, release under 

Parts of code are copyrighted by STMicroelectronics licensed under MCD-ST Liberty SW License Agreement V2
http://www.st.com/software_license_agreement_liberty_v2

This library must be used with STM8S_StdPeriph_Lib package which is provided free of charge by STMicroelectronics

Brief guide: 

- Two source files: stm8s_clk.c and stm8s_tim4.c must be added to project

- This code in tested on IAR. If using other compiler, the line "__IO uint32_t current_millis" might have to be put in the header file, and change to:
"volatile uint32_t current_millis"

- If your project is used with stm8s_it library, remember to comment out the function INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23) in stm8s_it.c
*/

#include "tim4millis.h"


//TIMER 4 initialization function without interrupt
void TIM4_init(void)
{
	/* TIM4 configuration:
	- TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 16 so the TIM4 counter
	clock used is 16 MHz / 128 = 125kHz 
	- With 125kHz we can generate time base:
	  max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
	  min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
	- In this example we need to generate a time base equal to 1 ms
	so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 = 0x7C*/
	
	TIM4_DeInit();
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 0x7C);
	TIM4_Cmd(DISABLE);
	TIM4_PrescalerConfig(TIM4_PRESCALER_128, TIM4_PSCRELOADMODE_IMMEDIATE);
	TIM4_SetCounter(0x00);
	TIM4_ITConfig(TIM4_IT_UPDATE, DISABLE);
	

	

	/* Enable TIM4 */
	TIM4_Cmd(ENABLE);
  
}