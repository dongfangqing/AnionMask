#ifndef _STM32_CHIPID_H_
#define _STM32_CHIPID_H_


#include "stm32f0xx.h"




uint8_t Get_ChipID(uint32_t* CPU_ID);



uint32_t BLE_UserNum_Val(uint32_t* CPU_ID);


#endif
