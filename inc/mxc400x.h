#define MXC4005_REG_INT_CLR0		0x00

#define MXC4005_REG_INT_BITCLR		0x00

#define MXC4005_REG_INT_CLR1		0x01
#define MXC4005_REG_INT_CLR1_BIT_DRDYC	0x01

#define MXC4005_REG_XOUT_UPPER		0x03					//X-axis acceleration output MSB
#define MXC4005_REG_XOUT_LOWER		0x04					//X-axis acceleration output LSB
#define MXC4005_REG_YOUT_UPPER		0x05
#define MXC4005_REG_YOUT_LOWER		0x06
#define MXC4005_REG_ZOUT_UPPER		0x07		
#define MXC4005_REG_ZOUT_LOWER		0x08
#define MXC4005_REG_TOUT					0x09
#define MXC4005_REG_INT_MASK0			0x0A
#define MXC4005_REG_INT_ENABLE_MASK0 0x0F

#define MXC4005_REG_INT_MASK1			0x0B
#define MXC4005_REG_INT_MASK1_BIT_DRDYE	0x01

#define MXC4005_REG_DETECTION			0x0C


#define MXC4005_REG_CONTROL				0x0D
#define MXC4005_REG_DEVICE_ID			0x0E
#define MXC4005_REG_WHO_AM_I			0x0F
#define MXC4005_I2C_ADDRESS				0x2A

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

uint16_t convert(uint16_t hi, int16_t low)
{
	int16_t v = 0;
	v = (hi<<4) + (low>>4);
	
	if(v >= 2048)
	{
		v = v - 1;
		v = ~ v;
		v = v & 4095;
	}
	return v;
}

void mxc400_init(void)
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