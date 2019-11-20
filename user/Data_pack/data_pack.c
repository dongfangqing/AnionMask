#include "data_pack.h"
#include "SHT2X.h"
#include "Adc_Device.h"
#include "i2c_driver.h"

uint8_t DevSeqByte = 0;
extern uint8_t Air_Quality;
uint8_t Pack_IntegerData(uint8_t buff[])
{
	uint8_t array[17] = {0};
	uint8_t data_l = 0,data_h = 0;
	uint16_t length = 13;
	uint16_t i = 0;
	
	uint16_t data_temp;
	uint16_t data_hum;
	float temp_value;
	float hum_value;
	float BattryVoltage;
//	uint16_t value = 0;	
	
	SHT2x_SoftReset();
	if(!I2C_CheckDevice(0x80))
	{
		//printf("SHT20 OK!!!\r\n");
		SHT2x_MeasurePoll(TEMP, &data_temp);
		SHT2x_MeasurePoll(HUMIDITY, &data_hum);
		temp_value = SHT2x_CalcTemperatureC(data_temp);	
		hum_value = SHT2x_CalcRH(data_hum);
	}
	else 
	{
		temp_value = 0.0;
		hum_value = 0.0;
	}
	
	//printf("温度：%6.2f，湿度：%6.2f\r\n",temp_value,hum_value);
	
	//value = Get_Adc(4);			//获取传感器ADC，发送给蓝牙
	BattryVoltage = Get_BattryVoltage();
	
	
	//start flag
	array[0] = 0xff;
	array[1] = 0xff;

	//lenth
	data_l = length&0xff;
	data_h = (length>>8)&0xff;
	array[2] = data_h;
	array[3] = data_l;

	if(temp_value >= 0)
	{
		//cmd	温度正
		array[4] = DEV_TEMP_POSITIVE;

		data_h = (uint8_t)temp_value;	
		data_l= (uint8_t)((temp_value-data_h)*100);	
		array[5] = data_h;
		array[6] = data_l;
	}
	else
	{
		//cmd	温度负
		array[4] = DEV_TEMP_NEGATIVE;

		data_h = (uint8_t)temp_value;	
		data_l= (uint8_t)((temp_value-data_h)*100);	
		array[5] = data_h;
		array[6] = data_l;
	}
	
	//湿度
	array[7] = DEV_HUM;

	data_h = (uint8_t)hum_value;	
	data_l= (uint8_t)((hum_value-data_h)*100);	
	array[8] = data_h;
	array[9] = data_l;
	
	//空气质量
	array[10] = DEV_AIR;

	data_l = Air_Quality&0xff;
	data_h = (Air_Quality>>8)&0xff;
	array[11] = data_h;
	array[12] = data_l;
	
	//电池电压
	array[13] = DEV_VOLTAGE;

	data_h = (uint8_t)BattryVoltage;	
	data_l= (uint8_t)((BattryVoltage-data_h)*100);	
	array[14] = data_h;
	array[15] = data_l;
	
	//CRC
	data_h = array[2];
	for(i = 3;i < 16;i++)
	{
		data_h = data_h^array[i];
	}

	array[16] = data_h;

	for(i = 0;i < 17;i++)
	{
		buff[i] = array[i];
	}

	return i;
}




uint8_t Untie_DataPack(uint8_t buff[])
{
	uint16_t lenth = 0;
	uint16_t i = 0;
	uint8_t crc_data = 0;
	
	if((buff[0] == 0xff)&&(buff[1] == 0xff))
	{
		lenth = 0xffff&((buff[2]<<8)|buff[3]);

		crc_data = buff[2];
		for(i = 0;i < lenth+2-1-1;i++)
		{
			crc_data = crc_data^buff[i+3];
		}

		if(crc_data != buff[lenth+2+1])
		{
			return 0;
		}

		DevSeqByte = buff[5];

		return buff[4];
	}

	return 0;
}



