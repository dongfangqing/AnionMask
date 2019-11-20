#ifndef __KEY_DRIVER__
#define __KEY_DRIVER__


#include "stm32f0xx.h"
#include "SysTick.h"


#define KEY_DOWN		0
#define KEY_UP			1



#define	NO_KEY			0xff


//#define KEY_BASE_CNT  5
//#define KEY_LONG_CNT  (75)
//#define KEY_HOLD_CNT  15

#define KEY_BASE_CNT  35
#define KEY_LONG_CNT  (150)
#define KEY_HOLD_CNT  50



////////////////IO Key//////////////////////
#define IOKEY_SHORT		\
						MSG_IOKEY_SHORT,\
						NO_MSG,\
						NO_MSG,
							
							

//长按
#define IOKEY_LONG		\
						MSG_IOKEY_LONG,\
						NO_MSG,\
						NO_MSG,



//连按
#define IOKEY_HOLD		\
						MSG_IOKEY_HOLD,\
						NO_MSG,\
						NO_MSG,

//长按抬起
#define IOKEY_LONG_UP	\
						MSG_IOKEY_LONG_UP,\
						NO_MSG,\
						NO_MSG,

typedef enum
{
	MODE_ADKEY = 1,
	MODE_IOKEY,
}_KEY_MODE;





void key_GpioConfig(void);
//uint8_t Scan_key(void);

uint8_t Get_KeyStatus(void);

void Keydriver_Scan(void);


#endif


