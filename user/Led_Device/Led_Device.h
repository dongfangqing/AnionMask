#ifndef _LED_DEVICE_H_
#define _LED_DEVICE_H_


#include "stm32f0xx.h"
#include "SysTick.h"

#define ON		0
#define	OFF		1

#define		LED(a)		if(a)	GPIO_SetBits(GPIOA,GPIO_Pin_2);\
						else	GPIO_ResetBits(GPIOA,GPIO_Pin_2)
							
void Led_GpioConfig(void);
void led_ctrl(uint8_t num);
void led_lowbattry(void);						

#endif
