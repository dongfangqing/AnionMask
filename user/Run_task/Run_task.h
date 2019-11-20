#ifndef _RUN_TASK_H_
#define _RUN_TASK_H_

#include "stm32f0xx.h" 


typedef enum
{
	MSG_0 = 0,
	MSG_1,
	MSG_2,
	MSG_3,
	MSG_4,
	MSG_5,
	MSG_6,
	MSG_7,
	MSG_8,
	MSG_9,

	MSG_30MS,
	MSG_100MS,
	MSG_200MS,
	MSG_250MS,
	MSG_HALF_SECOND,
	MSG_800MS,
	
	MSG_IOKEY_SHORT,
	MSG_IOKEY_HOLD,
	MSG_IOKEY_LONG,
	MSG_IOKEY_LONG_UP,
	
	MSG_BLE_CONNECT,
	MSG_GET_DATA,

	MSG_DATA_PACK,
	MSG_SHUTDOWN,
	MSG_LOW_BATTRY,
	MSG_USB_DET,

	NO_MSG = 0xff
}_TASK_MSG;



/***********************************************************/


uint8_t Clear_Buff(uint8_t buf[],uint8_t len);

void Run_Start(void);



void AnionMask_Power_off(void);
uint8_t AnionMask_usb_det_status(void);

#endif


