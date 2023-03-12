/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
 
//Libraries
#include "stm8s.h"
#include "stm8s_conf.h"
#include "stm8s103_i2c.h"
#include "mxc400x.h"
#include "stm8s_gpio.h"
#include "stm8s103_serial.h"


//Inicializacion de puerto B y C
void GPIO_setup(void)
{   
	GPIO_DeInit(GPIOB);
	GPIO_Init(GPIOB,((GPIO_Pin_TypeDef)(SCL_pin | SDA_pin)),GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_DeInit(GPIOC);
	GPIO_Init(GPIOC, Led, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(GPIOC, signal, GPIO_MODE_OUT_PP_LOW_SLOW);
}

//Configuración del clock interno a 8Mhz.
//Se habilita el clock para I2c.
void clock_setup(void)
{
     CLK_DeInit();
		 //CLK_HSECmd(DISABLE);
		 //CLK_LSICmd(DISABLE);
     CLK_HSICmd(ENABLE);
     while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE);
     CLK_ClockSwitchCmd(ENABLE);
     CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
     CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV8);               
     CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);         
     CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
}

main()
{
	
	delay(T_wait);
	clock_setup();
	//Serial_begin(9600);
	GPIO_setup();
	GPIO_WriteLow(GPIOC, signal);
	mxc400_init();
	
	X_Hant = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_XOUT_UPPER);
	X_Lant = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_XOUT_LOWER);
		
	Y_Hant = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_YOUT_UPPER);
	Y_Lant = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_YOUT_LOWER);
	
	Z_Hant = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_ZOUT_UPPER);
	Z_Lant = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_ZOUT_LOWER);
	
	Xant = convert(X_Hant, X_Lant);
	Yant = convert(Y_Hant, Y_Lant);
	Zant = convert(Z_Hant, Z_Lant);
	
while (1)
{		
	X_H = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_XOUT_UPPER);
	X_L = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_XOUT_LOWER);

	Y_H = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_YOUT_UPPER);
	Y_L = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_YOUT_LOWER);
	
	Z_H = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_ZOUT_UPPER);
	Z_L = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_ZOUT_LOWER);
	
	X = convert(X_H, X_L);
	Y = convert(Y_H, Y_L);
	Z = convert(Z_H, Z_L);
	
	difX = diference(X, Xant);
	difY = diference(Y, Yant);
	difZ = diference(Z, Zant);
/*
	Serial_print_string ("X: ");
	Serial_print_int(difX);
	Serial_newtab();
	Serial_print_string (" Y: ");
	Serial_print_int(difY);
	Serial_newtab();
	Serial_print_string (" Z: ");
	Serial_print_int(difZ);
	Serial_newline();
*/
	if((difX >= X_TH) || (difY >= Y_TH) || (difZ >= Z_TH))
	{
		GPIO_WriteLow(GPIOC, Led);
		GPIO_WriteHigh(GPIOC, signal);
		delay(T_signal);
	}
	else
	{
		GPIO_WriteHigh(GPIOC, Led);
		GPIO_WriteLow(GPIOC, signal);		
	}
	
	Xant = X;
	Yant = Y;
	Zant = Z;
}
}

