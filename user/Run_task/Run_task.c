#include "Run_task.h"
#include "Data_Structure.h"
#include "data_pack.h"
#include "usart_device.h"
#include "SysTick.h"
#include "key_driver.h"
#include "Led_Device.h"
#include "SHT2X.h"
#include "Bluetooth.h"
#include "Adc_Device.h"
#include "usb_Det.h"
#include "Adc_Device.h"
#include "i2c_driver.h"
#include "data_pack.h"
#include "TGS2600_driver.h"
#include "STM32_ChipID.h"
#include "ST_string.h"
#include "LowPower.h"
#include "power_ctl.h"

#if 0
static void DevCmd_Msg(uint8_t cmd)
{
	switch(cmd)
	{
		case DEV_BLE:
			put_msg_Fifo(MSG_BLE_CONNECT);
			break;
		case GET_DATA:
			put_msg_Fifo(MSG_GET_DATA);
			break;
		
	}
}

#endif



uint8_t Clear_Buff(uint8_t buf[],uint8_t len)
{
	uint8_t i = 0;

	for(i = 0;i < len;i++)
	{
		buf[i] = 0;
	}

	return i;
}


uint8_t Air_Quality;

void Run_Task(void)
{
	uint8_t msg = NO_MSG;
//	uint8_t flag = 0;
	uint8_t BLE_flag = 0;
//	uint8_t key_flag = 0;

//	uint8_t battry_voltage[2];
	uint8_t sensor_value[20];
	uint8_t lowbattry_flag = 0;
	uint16_t lowbattry_count = 0;
	uint8_t half_sec_count = 0;
	uint8_t usb_det_count= 0;
//	uint8_t key_long_count = 0;

#if 1	
	uint16_t data_temp;
	uint16_t data_hum;
	float temp_value;
	float hum_value;
	float BattryVoltage;

#endif
	
	uint8_t sensor_gather = 0;
	uint8_t sensor_getValue = 0;
	
	uint8_t BLE_start_flag = 0;
	uint8_t BLE_init_time = 0;
	uint8_t BLE_connect_flag = 0;
	
	uint32_t CPU_ID[3];
	uint32_t BLE_ID;
	
//	printf("Run_Task!!!\r\n");
	while(1)
	{
		msg = NO_MSG;
		msg = get_msg_Fifo();

		switch(msg)
		{
			case MSG_BLE_CONNECT:
				//led_ctrl(5);	//蓝牙连接成功，闪5次
				//LED(ON);
				BLE_flag = 1;
				BLE_connect_flag = 1;
				TGS2600_Power_ctl(ON);
				break;
			case MSG_GET_DATA:
	
				//Pack_IntegerData(sensor_value);
				//Bluetooth_Sendmsg(sensor_value,20);
				break;
			case MSG_DATA_PACK:
				//进行数据解包
				//DevCmd_Msg();
				//Clear_USART_Buff(USART1_BUF);
				break;
			case MSG_LOW_BATTRY:
				lowbattry_flag = 1;
				break;
			case MSG_USB_DET:
				{
					uint8_t msg = NO_MSG;
					uint8_t status = 0;
					#ifdef USART1_BEBUG
					printf("USB DET ON !!!\r\n");
					#endif
					LED(OFF);
					while(1)
					{
						msg = NO_MSG;
						msg = get_msg_Fifo();
						switch(msg)
						{
							case MSG_HALF_SECOND:
								#ifdef USART1_BEBUG
								printf("MSG_HALF_SECOND!!!\r\n");
								#endif
								if(!AnionMask_usb_det_status())
								{
									status = 1;
								}
								break;
							default:
								break;
						}
						if(status)
							break;
					}
					
					
					In_Stop_Mode();
					#ifdef USART1_BEBUG
					printf("PWR_EnterSTOPMode ------- !!!\r\n");
					#endif
					
					NVIC_SystemReset();
				}
				break;
			case MSG_SHUTDOWN:
				#if 0
				printf("MSG_SHUTDOWN!!!\r\n");
				#endif			
				In_Stop_Mode();
				#ifdef USART1_BEBUG
				printf("PWR_EnterSTOPMode ------- !!!\r\n");
				#endif

				NVIC_SystemReset();
				while(1);
//				return;
			case MSG_IOKEY_LONG:
				#ifdef USART1_BEBUG
				printf("MSG_IOKEY_LONG!!!\r\n");
				#endif
				#if 1
//				key_long_count = 1;
//				if(key_long_count >= 2)
//				{
//					key_long_count = 0;
//					Clear_msg_Fifo();
//					put_msg_Fifo(MSG_SHUTDOWN);
//					break;
//				}
				#else
				
				#endif
				break;
			case MSG_IOKEY_LONG_UP:
				#ifdef USART1_BEBUG
				printf("MSG_IOKEY_LONG_UP!!!\r\n");
				#endif
//				if(key_long_count >= 2)
//				{
//					key_long_count = 0;
					Clear_msg_Fifo();
					put_msg_Fifo(MSG_SHUTDOWN);
//				}
				break;
			#if 1
			case MSG_30MS:
				//printf("MSG_30MS!!!\r\n");
				Save_Rs_Val(sensor_getValue);
				sensor_getValue++;
				if(sensor_getValue == 8)
				{
					sensor_getValue = 0;
				}
				if(st_strcmp(Usart1_buff,"Wechat has connected.\r\n") == 0)
				{
					//printf("微信连接成功\r\n");
					put_msg_Fifo(MSG_BLE_CONNECT);
					Clear_USART_Buff(USART1_BUF);
				}
				else if(st_strcmp(Usart1_buff,"TTM:DISCONNET\r\n") == 0)
				{
					//printf("微信断开成功\r\n");
					BLE_flag = 0;
					Clear_USART_Buff(USART1_BUF);
					Bluetooth_RF_BM_S02_Connect_Mode();	
				}
				else
				{
					Clear_USART_Buff(USART1_BUF);
				}
				break;
			#endif
			case MSG_200MS:
//				#ifdef USART1_BEBUG
//				printf("MSG_200MS!!!\r\n");
//				#endif
				break;
			case MSG_250MS:
//				#ifdef USART1_BEBUG
//				printf("MSG_250MS!!!\r\n");
//				#endif
				sensor_gather++;
				if(sensor_gather == 8)
				{
					sensor_gather = 0;
					Set_MaxR_Val();
					//printf("污染等级：%d\r\n",PolluteClass_Judge());//等级判断
					Air_Quality = PolluteClass_Judge();
					
				}
				break;
			case MSG_HALF_SECOND:
				//printf("MSG_HALF_SECOND!!!\r\n");
				half_sec_count++;
				BLE_init_time++;
				
				
				if((BLE_init_time == 8) && (BLE_start_flag == 0) )
				{
					BLE_start_flag = 1;	
					Get_ChipID(CPU_ID);
					BLE_ID = BLE_UserNum_Val(CPU_ID);
					//修改蓝牙名称
					//Bluetooth_RF_BM_S02_change_name(BLE_ID);
					//蓝牙发送数据到H5页面
					Bluetooth_RF_BM_S02_MSGto_H5();
					//蓝牙进入连接模式
					 Bluetooth_RF_BM_S02_Connect_Mode();	
				}
				//微信连接成功
				if(BLE_flag == 1)
				{
					if(half_sec_count%2 == 0)
					{
//						Pack_IntegerData(sensor_value);
//						//Bluetooth_Sendmsg("123",3);
//						Bluetooth_Sendmsg(sensor_value,17);
						//printf("hello\r\n");
						if(BLE_connect_flag <= 5)
						{
							GPIOA->ODR ^= GPIO_Pin_2;
							BLE_connect_flag++;
						}
						else
						{
							Pack_IntegerData(sensor_value);
//						//Bluetooth_Sendmsg("123",3);
							Bluetooth_Sendmsg(sensor_value,17);
							LED(ON);
						}
					}	
				}
#if 0
				SHT2x_SoftReset();
				if(!I2C_CheckDevice(0x80))
				{
					printf("SHT20 OK!!!\r\n");
				}
				else printf("SHT20 ERROR!!!\r\n");
				SHT2x_MeasurePoll(TEMP, &data_temp);
				SHT2x_MeasurePoll(HUMIDITY, &data_hum);
				temp_value = SHT2x_CalcTemperatureC(data_temp);	
				hum_value = SHT2x_CalcRH(data_hum);
				printf("温度:%6.2f,湿度:%6.2f\r\n",temp_value,hum_value);

				printf("ADC:%d %d\r\n",Get_Adc(3),Get_Adc(4));
				printf("电池电压:%.2f\r\n",Get_BattryVoltage());
				printf("空气电压:%.2f\r\n",Get_Voltage(Get_Adc(4)));
#endif

				if(lowbattry_flag)//低电量提示
				{
					if(half_sec_count%2 == 0)
						LED(ON);
					else LED(OFF);
				}
				if(Get_BattryVoltage() <= 3.40)
				{
					lowbattry_count++;
					if(lowbattry_count > 20)
					{
						put_msg_Fifo(MSG_LOW_BATTRY);
					}
					
					if(lowbattry_count > 360)
					{
						put_msg_Fifo(MSG_SHUTDOWN);
					}
				}
				else lowbattry_count = 0;
				
				if(AnionMask_usb_det_status())
				{
					usb_det_count++;
					if(usb_det_count > 10)
					{
						Clear_msg_Fifo();
						put_msg_Fifo(MSG_USB_DET);
						usb_det_count = 0;
					}
				}
				else usb_det_count = 0;
				
//				if(key_long_count >= 1)
//					key_long_count++;
				break;
			default:
			
				break;
		}
	}
}



void Run_Start(void)
{
	ADC_InitConfig();
//	led_ctrl(2);
	LED(ON);

	I2C_GPIOInitConfig();
	delay_ms(15);
#if 1
	if(!I2C_CheckDevice(0x80))
	{
		#ifdef USART1_BEBUG
		printf("SHT20 OK!!!\r\n");
		#endif
	}
	#ifdef USART1_BEBUG
	else printf("SHT20 ERROR!!!\r\n");
	#endif
#endif 
	
	Anion_Power_ctl(ON);
	TGS2600_Power_ctl(ON);
	Clear_msg_Fifo();
	
	Run_Task();
}

