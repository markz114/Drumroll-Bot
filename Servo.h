#include "Definitions.h"
#include "LED.h"

#ifndef SERVO_H_
#define SERVO_H_

	void servo_init(void);
	void servo_setup_timers(void);
	int servo_get_high(int angle);
	int servo_get_low(int high_load_val);
	
	void servo_hit(int servo_num);
	
	extern volatile int servo1_angle;
	extern volatile int servo2_angle;
	
	extern volatile int tap_dat_1;
	extern volatile int tap_dat_2;

	extern volatile int intflag;
	extern volatile int intflag2;

#endif //SERVO_H_
