#include "MK64F12.h"

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_
	void delay(int delay);
	
#define BT1	2	//Port C
#define BT2 3 //Port C
#define BT3 1 //Port C
#define BT4	1 //Port C

#define SERVO1	0  //Port D 
#define SERVO2  2  //Port D

#define MAX_ANGLE  180 //In degrees
#define MIN_ANGLE  0	//In degrees
#define MILLISECOND  20970 //Clock cycles
#define PERIOD 419400 //20ms 

#define SERVO_HIT 150
#define SERVO_NEUTRAL 90
#define TAP_LIMIT 10		//20ms times tap_limit = how long we hold servo down


#endif //Definitions_H_
