//==============================================================================
// S E N S I R I O N AG, Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//==============================================================================
// Project : SHT2x Sample Code (V1.2)
// File : SHT2x.c
// Author : MST
// Controller: NEC V850/SG3 (uPD70F3740)
// Compiler : IAR compiler for V850 (3.50A)
// Brief : Sensor layer. Functions for sensor access
//==============================================================================
//---------- Includes ----------------------------------------------------------
#include "SHT2x.h"
#include "i2c_driver.h"

//返回0复位成功
//===========================================================================
uint8_t SHT2x_SoftReset(void)
//===========================================================================
{
	uint8_t error=0; //error variable
	I2C_Start();
	I2C_SendByte(I2C_ADR_W); // I2C Adr
	error |= I2C_RecvACK();
	I2C_SendByte(SOFT_RESET); // Command
	error |= I2C_RecvACK();
	I2C_Stop();
	delay_ms(15); // wait till sensor has restarted
	return error;
}
//CRC8校验算法
//data[]:信息码
//Bytes：字节数
//checksum：校验和，与校验码进行对比
//==============================================================================
uint8_t SHT2x_CheckCrc(uint8_t data[], uint8_t Bytes, uint8_t checksum)
//==============================================================================
{
	uint8_t crc = 0;
	uint8_t byteCtr;
	uint8_t bit = 0;
	//calculates 8-Bit checksum with given polynomial
	for (byteCtr = 0; byteCtr < Bytes; ++byteCtr)
	{ 
		crc ^= (data[byteCtr]);
		for ( bit = 8; bit > 0; --bit)
		{ 
			if(crc & 0x80)
			{
				crc = (crc << 1) ^ POLYNOMIAL;
			}
			else 
			{
				crc = (crc << 1);
			}
		}
	}
	if (crc != checksum) return CHECKSUM_ERROR;
	else return 0;
}

//读用户寄存器
//===========================================================================
uint8_t SHT2x_ReadUserRegister(uint8_t *pRegisterValue)
//===========================================================================
{
//	uint8_t checksum; //variable for checksum byte
	uint8_t error=0; //variable for error code
	I2C_Start();
	I2C_SendByte(I2C_ADR_W);
	error |= I2C_RecvACK();
	I2C_SendByte(USER_REG_R);
	error |= I2C_RecvACK();
	I2C_Start();
	I2C_SendByte(I2C_ADR_R);
	error |= I2C_RecvACK();
	*pRegisterValue = I2C_RecvByte();
	I2C_SendACK(1);		//非应答位
	//checksum=I2c_ReadByte(NO_ACK);
	//error |= SHT2x_CheckCrc (pRegisterValue,1,checksum);
	I2C_Stop();
	return error;
}
//写用户寄存器
//===========================================================================
uint8_t SHT2x_WriteUserRegister(uint8_t *pRegisterValue)
//===========================================================================
{
	uint8_t error=0; //variable for error code
	I2C_Start();
	I2C_SendByte(I2C_ADR_W);
	error |= I2C_RecvACK();
	I2C_SendByte((USER_REG_W));
	error |= I2C_RecvACK();
	I2C_SendByte(*pRegisterValue);
	error |= I2C_RecvACK();
	I2C_Stop();
	return error;
}
/*
//读出数据不正常
//===========================================================================
uint8_t SHT2x_MeasureHM(etSHT2xMeasureType eSHT2xMeasureType, uint16_t *result)
//===========================================================================
{
	uint8_t checksum; //checksum
	uint8_t data[2]; //data array for checksum verification
	uint8_t error=0; //error variable
	uint16_t i; //counting variable
	//-- write I2C sensor address and command --
	I2C_Start();
	I2C_SendByte(I2C_ADR_W);// I2C Adr
	error |= I2C_RecvACK(); 
	switch(eSHT2xMeasureType)
	{ 
		case HUMIDITY: I2C_SendByte(TRIG_RH_MEASUREMENT_HM); error |= I2C_RecvACK();break;
		case TEMP : I2C_SendByte(TRIG_T_MEASUREMENT_HM);error |= I2C_RecvACK(); break;
		default: break;
	}
	//-- wait until hold master is released --
	I2C_Start();
	I2C_SendByte(I2C_ADR_R);
	error |= I2C_RecvACK(); 
	if(error != 0)
		printf("error SCL IN\r\n");
	error = 0;
	SCL_IN();
	//SCL=HIGH; // set SCL I/O port as input
	for(i=0; i<1000; i++) // wait until master hold is released or
	{ 
		delay_ms(1); // a timeout (~1s) is reached
		if (I2C_SCL_READ() ==1)
		{
			break;
		}
	}
	//-- check for timeout --
	if(I2C_SCL_READ()==0) 
		error |= TIME_OUT_ERROR;
	//-- read two data bytes and one checksum byte --
	if(error != 0)
		printf("TIME_OUT_ERROR\r\n");
	error = 0;
	SCL_OUT();
	data[0] = I2C_RecvByte();
	I2C_SendACK(0);		//应答位
	data[1] = I2C_RecvByte();
	I2C_SendACK(0);		//应答位
	checksum = I2C_RecvByte();
	I2C_SendACK(1);		//非应答位
	//-- verify checksum --
	error |= SHT2x_CheckCrc (data,2,checksum);
	if(error != 0)
		printf("checksum error\r\n");
	I2C_Stop();
	*result = 0;
	*result |= data[0];
	*result = (*result << 8)| data[1];
	return error;
}
*/
//===========================================================================
uint8_t SHT2x_MeasurePoll(etSHT2xMeasureType eSHT2xMeasureType, uint16_t *result)
//===========================================================================
{
	uint8_t checksum; //checksum
	uint8_t data[2]; //data array for checksum verification
	uint8_t error=0; //error variable
	uint16_t i=0; //counting variable
	//-- write I2C sensor address and command --
	I2C_Start();
	I2C_SendByte(I2C_ADR_W);// I2C Adr
	error |= I2C_RecvACK(); 
	switch(eSHT2xMeasureType)
	{ 	case HUMIDITY: I2C_SendByte(TRIG_RH_MEASUREMENT_POLL); error |= I2C_RecvACK(); break;
		case TEMP : I2C_SendByte(TRIG_T_MEASUREMENT_POLL);  error |= I2C_RecvACK(); break;
		default: break;
	}
	//-- poll every 10ms for measurement ready. Timeout afloater 20 retries (200ms)--
	do
	{ 
		I2C_Start();
		delay_ms(10); //delay 10ms
		if(i++ >= 20)
			break;
		I2C_SendByte(I2C_ADR_R);	
	} while(I2C_RecvACK() == ACK_ERROR);
	if (i>=20) 
		error |= TIME_OUT_ERROR;
	//-- read two data bytes and one checksum byte --
	data[0] = I2C_RecvByte();
	I2C_SendACK(0);		//应答位
	data[1] = I2C_RecvByte();
	I2C_SendACK(0);		//应答位
	checksum=I2C_RecvByte();
	I2C_SendACK(1);		//非应答位
	//-- verify checksum --
	error |= SHT2x_CheckCrc (data,2,checksum);
	I2C_Stop();
	*result = 0;
	*result |= data[0];
	*result = (*result << 8)| data[1];
#if 0
	printf("data[0] = 0x%x\r\n",data[0]);
	printf("data[1] = 0x%x\r\n",data[1]);
	printf("result = 0x%x\r\n",*result);
#endif
	return error;
}

//==============================================================================
float SHT2x_CalcRH(uint16_t RH)
//==============================================================================
{
	float humidityRH; // variable for result
	RH &= ~0x0003; // clear bits [1..0] (status bits)
	//-- calculate relative humidity [%RH] --
	humidityRH = -6.0 + 125.0/65536 * (float)RH; // RH= -6 + 125 * SRH/2^16
	return humidityRH;
}
//==============================================================================
float SHT2x_CalcTemperatureC(uint16_t Temp)
//==============================================================================
{
	float temperatureC; // variable for result
	Temp &= ~0x0003; // clear bits [1..0] (status bits)
	//-- calculate temperature [癈] --
	temperatureC= -46.85 + 175.72/65536 *(float)Temp; //T= -46.85 + 175.72 * ST/2^16
	return temperatureC;
}
/*
//==============================================================================
uint8_t SHT2x_GetSerialNumber(uint8_t SerialNumber[])
//==============================================================================
{
	uint8_t error=0; //error variable
	//Read from memory location 1
	I2C_Start();
	I2C_SendByte(I2C_ADR_W); //I2C address
	error |= I2C_RecvACK(); 
	I2C_SendByte(0xFA); //Command for readout on-chip memory
	error |= I2C_RecvACK(); 
	I2C_SendByte(0x0F); //on-chip memory address
	I2C_Start();
	I2C_SendByte(I2C_ADR_R); //I2C address
	error |= I2C_RecvACK(); 
	SerialNumber[5] = I2c_ReadByte(); //Read SNB_3
	I2C_SendACK(0);
	I2c_ReadByte(ACK); //Read CRC SNB_3 (CRC is not analyzed)
	u8SerialNumber[4] = I2c_ReadByte(ACK); //Read SNB_2
	I2c_ReadByte(ACK); //Read CRC SNB_2 (CRC is not analyzed)
	u8SerialNumber[3] = I2c_ReadByte(ACK); //Read SNB_1
	I2c_ReadByte(ACK); //Read CRC SNB_1 (CRC is not analyzed)
	u8SerialNumber[2] = I2c_ReadByte(ACK); //Read SNB_0
	I2c_ReadByte(NO_ACK); //Read CRC SNB_0 (CRC is not analyzed)
	I2c_StopCondition();
	//Read from memory location 2
	I2c_StartCondition();
	error |= I2c_WriteByte (I2C_ADR_W); //I2C address
	error |= I2c_WriteByte (0xFC); //Command for readout on-chip memory
	error |= I2c_WriteByte (0xC9); //on-chip memory address
	I2c_StartCondition();
	error |= I2c_WriteByte (I2C_ADR_R); //I2C address
	u8SerialNumber[1] = I2c_ReadByte(ACK); //Read SNC_1
	u8SerialNumber[0] = I2c_ReadByte(ACK); //Read SNC_0
	I2c_ReadByte(ACK); //Read CRC SNC0/1 (CRC is not analyzed)
	u8SerialNumber[7] = I2c_ReadByte(ACK); //Read SNA_1
	u8SerialNumber[6] = I2c_ReadByte(ACK); //Read SNA_0
	I2c_ReadByte(NO_ACK); //Read CRC SNA0/1 (CRC is not analyzed)
	I2c_StopCondition();
	return error;
}
*/

