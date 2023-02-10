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


//Defines
#define SDA_pin GPIO_PIN_5
#define SCL_pin GPIO_PIN_4
#define Led GPIO_PIN_3
#define signal GPIO_PIN_6

//Vibration sensor parameters
#define X_TH 50			
#define Y_TH 50
#define Z_TH 80

//Global variables
int16_t X_L = 0;
int16_t X_H = 0;
int16_t Y_L = 0;
int16_t Y_H = 0;
int16_t Z_L = 0;
int16_t Z_H = 0;

int16_t X = 0;
int16_t Y = 0;
int16_t Z = 0;

int16_t X_Lant = 0;
int16_t X_Hant = 0;
int16_t Y_Lant = 0;
int16_t Y_Hant = 0;
int16_t Z_Lant = 0;
int16_t Z_Hant = 0;

int16_t Xant = 0;
int16_t Yant = 0;
int16_t Zant = 0;

int16_t difX = 0;
int16_t difY = 0;
int16_t difZ = 0;
uint32_t time_old, time_now = 0;
uint8_t pp= 0;

void GPIO_setup(void)
{   
	GPIO_DeInit(GPIOB);
	GPIO_Init(GPIOB,((GPIO_Pin_TypeDef)(SCL_pin | SDA_pin)),GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_DeInit(GPIOC);
	GPIO_Init(GPIOC, Led, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(GPIOC, signal, GPIO_MODE_OUT_PP_LOW_SLOW);
}



void delay (int ms) //Function Definition 
{
int i =0 ;
int j=0;
for (i=0; i<=ms; i++)
{
	for (j=0; j<120; j++) // Nop = Fosc/4
	_asm("nop"); //Perform no operation //assembly code  
}
}

void mxc400_init()
{
	I2C_DeInit();
	I2C_Setup(MXC4005_I2C_ADDRESS);
	
	//Borrado de los registros de interrupcion
	I2C_ByteWrite(MXC4005_I2C_ADDRESS, MXC4005_REG_INT_CLR0, MXC4005_REG_INT_BITCLR);
	delay(150);
	I2C_ByteWrite(MXC4005_I2C_ADDRESS, MXC4005_REG_INT_CLR1, MXC4005_REG_INT_BITCLR);
	delay(150);
	//Configuraci?n de interrupciones
	I2C_ByteWrite(MXC4005_I2C_ADDRESS, MXC4005_REG_INT_MASK0, MXC4005_REG_INT_ENABLE_MASK0);
	delay(150);
	I2C_ByteWrite(MXC4005_I2C_ADDRESS, MXC4005_REG_INT_MASK1, MXC4005_REG_INT_MASK1_BIT_DRDYE);
	delay(150);
	
	//Configuracion de DETECTION
	I2C_ByteWrite(MXC4005_I2C_ADDRESS, MXC4005_REG_DETECTION, MXC4005_REG_INT_BITCLR);
	
	//Configuracion Operational mode
	I2C_ByteWrite(MXC4005_I2C_ADDRESS, MXC4005_REG_CONTROL, MXC4005_REG_INT_BITCLR);

}

uint16_t convert(uint16_t v)
{
	if(v >= 2048)
	{
		v = v - 1;
		v = ~ v;
		v = v & 4095;
	}
	return v;
}

uint16_t diference(uint16_t current, uint16_t ant )
{
	uint16_t dif = 0;
	if(current < ant)
	{
		dif = (ant - current);
	}
	else
	{
		dif = (current - ant);
	}
	return dif;
}
void clock_setup(void)
{
	CLK_DeInit();
	
	CLK_HSECmd(DISABLE);
	CLK_LSICmd(DISABLE);
	CLK_HSICmd(DISABLE);
	while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY)== FALSE);
	
	CLK_ClockSwitchCmd(ENABLE);
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV4);
	
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);
	
}
main()
{
	clock_setup();
	Serial_begin(9600);
	GPIO_setup();
	GPIO_WriteLow(GPIOC, signal);
	
	mxc400_init();
	
	X_Hant = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_XOUT_UPPER);
	X_Lant = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_XOUT_LOWER);
		
	Y_Hant = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_YOUT_UPPER);
	Y_Lant = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_YOUT_LOWER);
	
	Z_Hant = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_ZOUT_UPPER);
	Z_Lant = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_ZOUT_LOWER);
	
	Xant = (X_Hant<<4)+(X_Lant>>4);
	Yant = (Y_Hant<<4)+(Y_Lant>>4);
	Zant = (Z_Hant<<4)+(Z_Lant>>4);
	
	
while (1)
{	
	
	
	
	X_H = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_XOUT_UPPER);
	X_L = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_XOUT_LOWER);

	Y_H = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_YOUT_UPPER);
	Y_L = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_YOUT_LOWER);
	
	Z_H = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_ZOUT_UPPER);
	Z_L = I2C_ByteRead(MXC4005_I2C_ADDRESS, MXC4005_REG_ZOUT_LOWER);
	
	X = (X_H << 4)+(X_L >> 4);
	Y = (Y_H<<4)+( Y_L >>4);
	Z = (Z_H<<4)+( Z_L >>4);
	
	X = convert(X);
	Y = convert(Y);
	Z = convert(Z);
	
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
		delay(100);
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

