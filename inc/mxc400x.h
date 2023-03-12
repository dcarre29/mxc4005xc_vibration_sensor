
//Registros MXC400

#define MXC4005_REG_INT_CLR0		0x00
#define MXC4005_REG_INT_BITCLR		0x00

#define MXC4005_REG_INT_CLR1		0x01
#define MXC4005_REG_INT_CLR1_BIT_DRDYC	0x01

#define MXC4005_REG_XOUT_UPPER		0x03				
#define MXC4005_REG_XOUT_LOWER		0x04					
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


//PinOut
#define SDA_pin GPIO_PIN_5
#define SCL_pin GPIO_PIN_4
#define Led GPIO_PIN_3
#define signal GPIO_PIN_6


//Vibration sensor parameters
//Límites de cada eje apartir del cual se detecta movimiento
#define X_TH 50														
#define Y_TH 50
#define Z_TH 80

#define T_wait   1000														//Tiempo que se espera para inicilizacion de la maquina
#define T_signal 100														//Tiempo de mantenimiento de la señal cuando se detecta vibracion


//Variables

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

//Function Definition

void delay (int ms) 
{
int i =0 ;
int j=0;
for (i=0; i<=ms; i++)
{
	for (j=0; j<331; j++) // Nop = Fosc/8
	_asm("nop"); //Perform no operation //assembly code  
}
}

//conversion de complemento a dos a decimal
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

//diferencia entre el valor anterior y el actual
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

//configuración del modo de operacion del mxc400
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