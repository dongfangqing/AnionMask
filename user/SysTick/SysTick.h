#ifndef _SYSTICK_H_
#define _SYSTICK_H_



#include "stm32f0xx.h"


#define	STK_CSR	(*(volatile unsigned long *)0xE000E010)
#define	STK_RVR	(*(volatile unsigned long *)0xE000E014)
#define	STK_CVR	(*(volatile unsigned long *)0xE000E018)



void SysTick_Init(uint8_t SYSCLK);
void delay_ms(uint16_t nms);
void delay_us(uint16_t nus);



#endif
