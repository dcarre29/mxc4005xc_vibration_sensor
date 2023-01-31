#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_i2c.h"


#define I2C_Speed 100000


void I2C_Setup(uint8_t address)
{
	I2C_Cmd(ENABLE);
  I2C_Init(I2C_Speed, address, I2C_DUTYCYCLE_2, I2C_ACK_CURR,I2C_ADDMODE_7BIT,(CLK_GetClockFreq()/1000000));
}

//Me parece que falta el byte de escritura
void I2C_ByteWrite(u8 I2C_Slave_Address, u8 reg_address, u8 iData)
{
	I2C_GenerateSTART(ENABLE);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C_Slave_Address, I2C_DIRECTION_TX);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(reg_address);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(iData);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(ENABLE);
}

u8 I2C_ByteRead(uint8_t I2C_Slave_Address, uint8_t ReadAddr)
{
	u16 pBuffer;
	while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
	I2C_GenerateSTART(ENABLE);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C_Slave_Address, I2C_DIRECTION_TX);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData((u8)(ReadAddr));
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTART(ENABLE);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C_Slave_Address, I2C_DIRECTION_RX);
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
	pBuffer = I2C_ReceiveData();
	I2C_AcknowledgeConfig(I2C_ACK_NONE);
	I2C_GenerateSTOP(ENABLE);
	return pBuffer;
}
