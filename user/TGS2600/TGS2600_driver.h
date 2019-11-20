#ifndef _TGS2600_DRIVER_H_
#define _TGS2600_DRIVER_H_

#include "Adc_Device.h"

#define G1		0
#define G2		0.08
#define G3		0.16
#define G4		-0.03
#define G5		0.05
#define G6		0.13
#define G7		0.03
#define G8		0.95
#define G9		0.87



#define VK		3.3		//ADC基准电压



typedef enum
{
	CLASS_0 = 0,
	CLASS_1,
	CLASS_2,
	CLASS_3,
}_POLLUTE_CLASS;


extern float MaxR_Val;//环境中Rs的最大值为基准值,单位为K欧

extern uint8_t PolluteClass;//污染等级


float Get_VRL_Val(void);
float Get_RS_Val(void);
void Save_Rs_Val(uint8_t num);
float Rs_filter(void);
void Set_MaxR_Val(void);
float PolluteDatum_K_Val(void);
uint8_t PolluteClass_Judge(void);


#endif
