#include "TGS2600_driver.h"



float MaxR_Val = 0.0;//环境中Rs的最大值为基准值,单位为K欧

uint8_t PolluteClass = 0;//污染等级
uint8_t back__PolluteClass = 0;//污染等级备份
float Rs_buffer[8] = {0.0};



float Get_VRL_Val(void)
{
	return Get_Voltage(Get_Adc(4));
}

	

//获取传感器阻值
//R = 10K欧
//Rs = (float)(VCC*R - VRL*R)/VRL
float Get_RS_Val(void)
{
	float Rs = 0.0;
	float vcc = Get_BattryVoltage();//电池电压
	float VRL = Get_VRL_Val();		//传感器电压
	
	Rs = (float)((vcc - VRL)*10.0)/VRL;
	
	return Rs;
}

void Save_Rs_Val(uint8_t num)
{
	float Rs = Get_RS_Val();
	
	Rs_buffer[num] = Rs;
}

float Rs_filter(void)
{
	uint8_t i = 0;
	float Rs_sum = 0.0;
	
	for(i = 0;i < 8;i++)
		Rs_sum += Rs_buffer[i];
	
	return (float)Rs_sum/8;
}

void Set_MaxR_Val(void)
{
	float Rs = Rs_filter();
	
	if(Rs > MaxR_Val)
		MaxR_Val = Rs_filter();
}


//计算污染判定基准值K
//K = VK/vcc
float PolluteDatum_K_Val(void)
{
	float K = 0.0;
	
	K = (float)VK/Get_BattryVoltage();
	
	if((K > 0.96)||(K < 0.06))
		K = 0.84;
	
	return K;
}




//污染等级判断
uint8_t PolluteClass_Judge(void)
{
	float K = PolluteDatum_K_Val();
	float ResistanceRatio = Rs_filter()/MaxR_Val;
	
	switch(PolluteClass)
	{
		case CLASS_0:
			if(ResistanceRatio <= K - G1)
			{
				PolluteClass = CLASS_1;
				back__PolluteClass = CLASS_0;
			}
			break;
		case CLASS_1:
			if((ResistanceRatio <= K - G2)&&(PolluteClass == CLASS_1)&&(back__PolluteClass == 0))
			{
				PolluteClass = CLASS_2;
				back__PolluteClass = CLASS_1;
				break;
			}
			if((ResistanceRatio >= K - G4)&&(PolluteClass == CLASS_1)&&(back__PolluteClass == 2))
			{
				PolluteClass = CLASS_0;
				back__PolluteClass = CLASS_1;
			}
			break;
		case CLASS_2:
			if((ResistanceRatio <= K - G3)&&(PolluteClass == CLASS_2)&&(back__PolluteClass == 1))
			{
				PolluteClass = CLASS_3;
				back__PolluteClass = CLASS_2;
				break;
			}
			if((ResistanceRatio >= K - G5)&&(PolluteClass == CLASS_2)&&(back__PolluteClass == 3))
			{
				PolluteClass = CLASS_1;
				back__PolluteClass = CLASS_2;
			}
			break;
		case CLASS_3:
			if(ResistanceRatio >= K - G6)
			{
				PolluteClass= CLASS_2;
				back__PolluteClass = CLASS_3;
			}
			break;
	}
	
	return PolluteClass;
}

	

