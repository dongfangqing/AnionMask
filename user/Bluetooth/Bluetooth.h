
#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__


#include "stm32f0xx.h"
#include "SysTick.h"

/***************************************************************/
#define GPIO_OUT_HIGH	1
#define GPIO_OUT_LOW	0

/*
#define BM_S02_ReSet(a)		if (a)	\
							GPIO_SetBits(GPIOB,GPIO_Pin_13);\
							else		\
							GPIO_ResetBits(GPIOB,GPIO_Pin_13)
*/

#define BM_S02_EN(a)		if (a)	\
							GPIO_SetBits(GPIOA,GPIO_Pin_6);\
							else		\
							GPIO_ResetBits(GPIOA,GPIO_Pin_6)
						
#define BM_S02_Check_Connect_State	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)

#define BRTS_State(a)		if (a)	\
							GPIO_SetBits(GPIOB,GPIO_Pin_1);\
							else		\
							GPIO_ResetBits(GPIOB,GPIO_Pin_1)
							
#define BCTS_State					GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)							


#define BM_S02_RestoreFactory(a)	if (a)	\
									GPIO_SetBits(GPIOB,GPIO_Pin_0);\
									else		\
									GPIO_ResetBits(GPIOB,GPIO_Pin_0)
							


void Bluetooth_RF_BM_S02_Port_GPIO_Init(void);
void Bluetooth_Sendmsg(uint8_t data[],uint8_t len);
void Bluetooth_RF_BM_S02_change_name(uint32_t id);
void Bluetooth_RF_BM_S02_MSGto_H5(void);
void Bluetooth_RF_BM_S02_change_Connect_time(void);
void Bluetooth_RF_BM_S02_change_BPS(void);
void Bluetooth_RF_BM_S02_Connect_Mode(void);
void Bluetooth_RF_BM_S02_Sleep_Mode(void);
void Bluetooth_RF_BM_S02_getMAC(void);
uint8_t Bluetooth_RF_BM_S02_Check_Connect_State(void);	
void Bluetooth_RF_BM_S02_RestoreFactory(int i);									
/*
void Bluetooth_RF_BM_S02_ReSet(void);


*/

#endif



