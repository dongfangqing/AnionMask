#include "SysTick.h"
#include "stm32f0xx.h"


static uint8_t  fac_us=0;//us延时倍乘数
static uint16_t fac_ms=0;//ms延时倍乘数

/*************************************************
函数: void SysTick_Init(uint8_t SYSCLK)
功能: 滴答定时器初始化
参数: SYSCLK:系统时钟,Cotex-M0最高时钟为48MHz
返回: SYSTICK的时钟固定为HCLK时钟的1/8
**************************************************/
void SysTick_Init(uint8_t SYSCLK)
{
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick->CTRL = 0xfffffffb;	// 关闭定时器,倒计时计数到0时产生异常,选择外部时钟源
	
	fac_us=SYSCLK/8;		    
	fac_ms=(uint16_t)fac_us*1000;
}



/*************************************************
函数: void delay_ms(uint16_t nms)
功能: 毫秒延时
参数: 延时n毫秒
返回: 无
**************************************************/
void delay_ms(uint16_t nms)
{
	uint32_t temp;
	
	SysTick->LOAD = (uint32_t)nms*fac_ms;	//装载计数值
	SysTick->VAL = 0x00;					//清空计数器
	SysTick->CTRL = 0x01;					//计数开始
	
	do
	{
		temp = SysTick->CTRL;
	}
	while((temp&0x01)&&(!(temp&(1<<16))));	//等待计时到达
	
	SysTick->CTRL = 0x00;					//关闭计数器
	SysTick->VAL = 0x00;					//清空计数器
}

/*************************************************
函数: void delay_us(uint16_t nus)
功能: 微秒延时
参数: 延时n微秒
返回: 无
**************************************************/
void delay_us(uint16_t nus)
{
	uint32_t temp;
	
	SysTick->LOAD = (uint32_t)nus*fac_us;	//装载计数值
	SysTick->VAL = 0x00;					//清空计数器
	SysTick->CTRL = 0x01;					//计数开始
	
	do
	{
		temp = SysTick->CTRL;
	}
	while((temp&0x01)&&(!(temp&(1<<16))));	//等待计时到达
	
	SysTick->CTRL = 0x00;					//关闭计数器
	SysTick->VAL = 0x00;					//清空计数器
}






