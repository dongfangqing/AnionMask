#include "usart_device.h"
#include "ST_string.h"
#include <stdio.h>

#ifdef USART1_ENABLE
int8_t Usart1_buff[USART1_BUFF] = {0};
USARTRecvData Usart1_RecvStat = 
{
	0,
	&Usart1_buff[0],
	&Usart1_buff[0],
};
#endif


#ifdef USART1_ENABLE

void Usart1_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_USARTCLKConfig(RCC_USART1CLK_PCLK);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_PinAFConfig(GPIOA ,GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA ,GPIO_PinSource10, GPIO_AF_1); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

int fputc(int ch, FILE *f)
{
		USART_SendData(USART1, (uint8_t) ch);

		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}



char usart1_getc(void)
{
	char temp = 0;
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)!= SET); 
	
	temp = USART_ReceiveData(USART1);

	return temp;
}

int usart1_getstr(unsigned char* buff,int len)
{
	int i = 0;

	for(i = 0;i < len;i++)
	{
		buff[i] = usart1_getc();
	}

	return len;
}

char usart1_putc(char ch)
{
	USART_SendData(USART1, (uint8_t) ch);
	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);		

	return (ch);
}

int usart1_putstr(unsigned char* buff,int len)
{
	int i = 0;
	
	for(i = 0;i < len;i++)
	{
		usart1_putc(buff[i]);
	}

	return len;
}

int Usart1_Sendstr(unsigned char* buff)
{
	int ret = 0;
	
	while(*buff)
	{
		usart1_putc(*buff);
		buff++;
		ret++;
	}
	
	return ret;
}

void put_data_u16(unsigned short dat)
{
	unsigned char temp;
	
	temp = dat/1000;
	usart1_putc(temp + '0');
	temp = dat/100%10;
	usart1_putc(temp + '0');
	temp = dat/10%10;
	usart1_putc(temp + '0');
	temp = dat%10;
	usart1_putc(temp + '0');
	
	usart1_putc('\n');
}

#endif

void Usart_NVIC_InitConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructconfig;

#ifdef USART1_ENABLE	
	NVIC_InitStructconfig.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructconfig.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructconfig.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructconfig);
//	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);
#endif
}

void Usart_InitConfig(void)
{
#ifdef USART1_ENABLE
	Usart1_InitConfig();
#endif
	
	Usart_NVIC_InitConfig();
}

void Clear_USART_Buff(USART_Buf_Select usart)
{
	if(usart == USART1_BUF)
	{
#ifdef USART1_ENABLE
		st_memset((int8_t*)Usart1_buff,(int8_t)0,USART1_BUFF);
		Usart1_RecvStat.Recvcount = 0;
		Usart1_RecvStat.Recv_start = &Usart1_buff[0];
		Usart1_RecvStat.Recv_end = &Usart1_buff[0];
#endif
	}
}


