#include "Run_task.h"
#include "key_driver.h"
#include "Data_Structure.h"

/*****************************************************************
**函数功能：按键GPIO初始化
******************************************************************/
void key_GpioConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


uint8_t Get_KeyStatus(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
}



uint8_t iokey_table[4][3] = {
	{IOKEY_SHORT},
	{IOKEY_LONG},
	{IOKEY_HOLD},
	{IOKEY_LONG_UP}
};


/****************************************************************************/

uint8_t io_key(void)
{
	uint8_t keyTemp;
	
	keyTemp = NO_KEY;
	
	if(Get_KeyStatus() == KEY_DOWN)
	{
		keyTemp = 0;
		return keyTemp;
	}
	
	return NO_KEY;
}



uint8_t Key_mode = 0;
uint8_t key_counter;   ///<???????????
uint8_t last_key = NO_MSG;
uint8_t msg_key = NO_MSG;
void Keydriver_Scan(void)
{
	uint8_t keyTemp = NO_KEY;
	uint8_t key_style = NO_KEY;
	
//	keyTemp = adkey(adkey_value);
	
	if(keyTemp == NO_KEY)
	{
		//Get IO Key Value
		keyTemp = io_key();
		if(keyTemp != NO_KEY)
		{
			Key_mode = MODE_IOKEY;	//IO Key
		}
	}
	else
		Key_mode = MODE_ADKEY;	//AD Key
	
	if(NO_KEY == keyTemp)
	{
		if (key_counter >= KEY_LONG_CNT)			 //????
		{
			key_style = 3;
		}
		else if (key_counter >= KEY_BASE_CNT)		//????
		{
			key_style = 0;
		}

		key_counter = 0;
	}
	else if (keyTemp != last_key)
	{ 
		key_counter = 0;
	}
	else
	{
		key_counter++;
		if (KEY_BASE_CNT == key_counter)			//??
		{
		}
		else if (KEY_LONG_CNT == key_counter)				//??
		{
			key_style = 1;
		}
		else if ((KEY_LONG_CNT + KEY_HOLD_CNT) == key_counter)			//??
		{
			key_style = 2;
			key_counter = KEY_LONG_CNT;
		}
	}
	
	if((NO_KEY != last_key) && (key_style != NO_KEY))
	{
		if(Key_mode == MODE_ADKEY)
		{
//			msg_key = adkey_table[key_style][last_key];
#if 0
			usart1_putc('Y');
			usart1_putc('Y');
			usart1_putc(key_style+'0');
			usart1_putc(last_key+'0');
//			usart1_putc(msg_key+'0');
			printf("%x",msg_key);
			usart1_putc('\n');
#endif
		}
		else if(Key_mode == MODE_IOKEY)									//IO Key
		{
			msg_key = iokey_table[key_style][last_key];
		}

		if (msg_key == NO_MSG)								//??
			goto _exit_keyScan;
	//		printf("=%x\n",msg_key);
			if(msg_key == MSG_IOKEY_LONG_UP)
				Clear_msg_Fifo();
			put_msg_Fifo(msg_key);
	//		printf("+%x\n",msg_key);
			msg_key = NO_MSG;
	}
	
_exit_keyScan:
	last_key = keyTemp;
//	usart1_putc('M');
//#endif
}



