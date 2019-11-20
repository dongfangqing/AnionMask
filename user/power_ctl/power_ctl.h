#ifndef _POWER_CTL_H_
#define _POWER_CTL_H_


#include "stm32f0xx.h"


void power_ctl_InitConfig(void);
void Anion_Power_ctl(uint8_t status);
void TGS2600_Power_ctl(uint8_t status);
void AIRclean_Power_ctl(uint8_t status);



#endif

