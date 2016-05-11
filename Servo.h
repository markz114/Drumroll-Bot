#include "Definitions.h"

#ifndef SERVO_H_
#define SERVO_H_

	void servo_init(void);
	void servo_setup_timers(void);
	int servo_get_high(int angle);
	int servo_get_low(int high_load_val);
	
	void servo_hit_1(void);
	void servo_hit_2(void);
	
	extern volatile int servo1_angle;


#endif //SERVO_H_
