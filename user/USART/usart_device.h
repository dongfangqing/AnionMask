#ifndef _USART_DEVICE_H_
#define _USART_DEVICE_H_



#include "stm32f0xx.h"
#include "stdio.h"


#define USART1_BUFF	512

#define USART1_ENABLE
#define USART1_BEBUG

/**************************************************************************/
typedef enum
{
	USART1_BUF,
	USART2_BUF,
	USART3_BUF
}USART_Buf_Select;


typedef struct arg_USARTRecvData
{
	int32_t Recvcount;
	int8_t *Recv_start;
	int8_t *Recv_end;
}USARTRecvData;

void Usart_InitConfig(void);

#ifdef USART1_ENABLE
extern int8_t Usart1_buff[USART1_BUFF];
extern USARTRecvData Usart1_RecvStat;
#endif

#ifdef USART1_ENABLE
void Usart1_InitConfig(void);
void Usart_NVIC_InitConfig(void);


int fputc(int ch, FILE *f);
int fgetc(FILE *f);

char usart1_getc(void);
int usart1_getstr(unsigned char* buff,int len);

char usart1_putc(char ch);
int usart1_putstr(unsigned char* buff,int len);
int Usart1_Sendstr(unsigned char* buff);
void put_data_u16(unsigned short dat);
void Clear_USART_Buff(USART_Buf_Select usart);

#endif

#endif
