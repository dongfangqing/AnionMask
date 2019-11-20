#ifndef __USB_DET_H__
#define __USB_DET_H__


#include "stm32f0xx.h"
#include "SysTick.h"


#define 	usb_State	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)

void usb_GpioConfig(void);
int usb_check(void);

#endif


