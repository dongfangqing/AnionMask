/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f0xx_it.h"
#include "usart_device.h"
#include "SPI_Device.h"
#include "Data_Structure.h"
#include "Run_task.h"
#include "key_driver.h"
#include "TGS2600_driver.h"      

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}



uint8_t ch;

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void USART1_IRQHandler(void)
{		
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
		USART_ClearITPendingBit (USART1, USART_IT_RXNE);
	    ch = USART_ReceiveData(USART1);
		*(Usart1_RecvStat.Recv_end) = ch;
	  	usart1_putc(ch);    //将接受到的数据直接返回打印
				
		Usart1_RecvStat.Recv_end++;
		Usart1_RecvStat.Recvcount++;
		
		if(USART1_BUFF <= Usart1_RecvStat.Recvcount)
		{
			Usart1_RecvStat.Recvcount = 0;
			Usart1_RecvStat.Recv_end = &Usart1_buff[0];
		}
		//Recv_Count++;
	} 	 
}

void SPI2_IRQHandler(void)
{
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


int time_out_flag = 0;
int time_out_flag_HalfSec = 0;
int time_out_flag_250MS = 0;
int time_out_flag_30MS = 0;
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
//		printf("TIM3_IRQHandler!!!\r\n");
		
		Keydriver_Scan();
		
		time_out_flag++;
		time_out_flag_HalfSec++;
		time_out_flag_250MS++;
		time_out_flag_30MS++;
		
		if((time_out_flag_30MS%15) == 0)
		{
			put_msg_Fifo(MSG_30MS);
			time_out_flag_30MS = 0;
		}
		
		if((time_out_flag%100) == 0)
		{
			put_msg_Fifo(MSG_200MS);
			time_out_flag = 0;
		}
		
		if((time_out_flag_HalfSec%250) == 0)
		{
			put_msg_Fifo(MSG_HALF_SECOND);
			time_out_flag_HalfSec = 0;
		}
		
		if((time_out_flag_250MS%125) == 0)
		{
			put_msg_Fifo(MSG_250MS);
			time_out_flag_250MS = 0;
		}
//		sensor_gather++;
//		if(sensor_gather%125)
//		{
//			Save_Rs_Val(sensor_gather/125-1);//可能会存在bug
//			
//			if(sensor_gather/125 == 8)
//			{
//				sensor_gather = 0;
//				Set_MaxR_Val();
//				
//				PolluteClass_Judge();//等级判断
//			}
//		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}


void EXTI4_15_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		printf("EXTI4_15_IRQHandler(===) !!!\r\n");
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

void EXTI0_1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		printf("EXTI0_1_IRQHandler(===) !!!\r\n");
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
