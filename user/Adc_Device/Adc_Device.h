#ifndef _ADC_DEVICE_H_
#define _ADC_DEVICE_H_



#include "stm32f0xx.h"


#define ADC1_DR_Address    0x40012440


void ADC_InitConfig(void);
void filter(uint8_t num);
uint16_t Get_Adc(uint8_t channel);
float Get_Voltage(uint16_t adc_value);

float Get_BattryVoltage(void);



#endif
