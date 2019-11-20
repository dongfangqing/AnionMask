#ifndef _DATA_PACK_H_
#define _DATA_PACK_H_

#include "stm32f0xx.h"


#if 0
typedef enum
{
	0x10,	//Accelerometer
	0x11,	//Gyroscope
	0x12,	//Magnetometer
	0x13,	//euler
	0x14,	//quaternion
	0x15,	//linear_accel
	0x16,	//gravity
	0x17,	//temperature
}DEV_TYPE;

#else

typedef enum
{
	DEV_TEMP_POSITIVE = 0X10,
	DEV_TEMP_NEGATIVE,
	DEV_HUM,
	DEV_AIR,
	DEV_VOLTAGE,
}DEV_TYPE;

#endif

uint8_t Pack_IntegerData(uint8_t buff[]);

#endif



