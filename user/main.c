/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "Led_Device.h"
#include "SysTick.h"
#include "usart_device.h"
#include "SPI_Device.h"
#include "OLED_Device.h"
#include "Adc_Device.h"
#include "i2c_driver.h"
#include "SHT2X.h"
#include "Run_task.h"
#include "tim_driver.h"
#include "key_driver.h"
#include "Data_Structure.h"
#include "Bluetooth.h"
#include "LowPower.h"
#include "power_ctl.h"

#if 0
/*************************************************
函数: void RCC_Configuration(void)
功能: 配置系统时钟,选择HSI作为时钟源,PLL为系统时钟源
参数: 无
返回: 无
**************************************************/
void RCC_Configuration(void)
{
	RCC_DeInit();
	RCC_HSICmd(ENABLE);
	
	FLASH_PrefetchBufferCmd(ENABLE);
	FLASH_SetLatency(FLASH_Latency_1);
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLKConfig(RCC_HCLK_Div1);
	
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
	RCC_PLLCmd(ENABLE);
	
	
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	while(RCC_GetSYSCLKSource() != 0x08);
}
#else
/*************************************************
函数: void RCC_Configuration(void)
功能: 配置系统时钟,选择HSE作为时钟源,PLL为系统时钟源
参数: 无
返回: 无
**************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(ENABLE);
		FLASH_SetLatency(FLASH_Latency_1);
		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLKConfig(RCC_HCLK_Div1);
		
		RCC_PLLConfig(RCC_PLLSource_HSE, RCC_PLLMul_6);
		RCC_PLLCmd(ENABLE);
		
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		while(RCC_GetSYSCLKSource() != 0x08);
	}
}

#endif

void AnionMask_Power_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);
}

void AnionMask_Power_on(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
}

void AnionMask_Power_off(void)
{
	LED(OFF);
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);
	
}

void AnionMask_usb_det_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t AnionMask_usb_det_status(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15);
}



int main(void)
{	
	RCC_Configuration();
	SysTick_Init(48);
	Usart_InitConfig();
	
	#ifdef USART1_BEBUG
	printf("main()\r\n");
	#endif
	printf("hello main\r\n");
	TIM_Init();
	power_ctl_InitConfig();
	key_GpioConfig();
	AnionMask_Power_Init();
	Led_GpioConfig();
	LED(ON);
	printf("hello main1\r\n");
//	while(1);
	
	Bluetooth_RF_BM_S02_Port_GPIO_Init();
//	Bluetooth_RF_BM_S02_MSGto_H5();
	AnionMask_Power_on();
//	while(1)
//	{
//		printf("USB +++++ ON ===!!!\r\n");
//		delay_ms(1000);
//	}
	
	
	AnionMask_usb_det_Init();
	
	
	SeqQueue_Init();
	Clear_msg_Fifo();
	
	#ifdef USART1_BEBUG
	printf("USB DET ON ===!!!\r\n");
	#endif
	
//	TGS2600_Power_ctl(ON);
	AIRclean_Power_ctl(ON);
	if(AnionMask_usb_det_status())
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
	}
	
	#ifdef USART1_BEBUG
	printf("BAT !!!\r\n");
	#endif
	{
		uint8_t msg = NO_MSG;
		uint8_t count = 1;
		uint8_t led_count = 1;
		while((Get_KeyStatus() == KEY_DOWN))
		{
			msg = NO_MSG;
			msg = get_msg_Fifo();
			switch(msg)
			{
				case MSG_HALF_SECOND:
					#ifdef USART1_BEBUG
					printf("MSG_HALF_SECOND!!!\r\n");
					#endif
					if(count < 4)
					{
						count++;
					}
					else
					{
						printf("count>4\r\n");
						led_count++;
						if(led_count%2 == 0)
						{							
							LED(OFF);
						}
						else
						{
							LED(ON);
						}
					}
					break;
				default:
					break;
			}
		}
		
		printf("RUN!!!\r\n");
		if((Get_KeyStatus() == KEY_UP)&&(count >= 2))
		{
			#ifdef USART1_BEBUG
			printf("RUN ST!!!\r\n");
			#endif
			AnionMask_Power_on();
			
			Run_Start();
		}
		
		#ifdef USART1_BEBUG
		printf("PWR_EnterSTOPMode +++++++!!!\r\n");
		#endif
//		while(1);
//		printf("PWR_EnterSTOPMode +++++++!!!\r\n");
		In_Stop_Mode();
		#ifdef USART1_BEBUG
		printf("PWR_EnterSTOPMode ------- !!!\r\n");
		#endif
		
		NVIC_SystemReset();
		
		while(1)
		{
		}
	}
}

