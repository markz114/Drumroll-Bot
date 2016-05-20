/*
*
*		Servo.c
*
*		Created: April 30 2016
*		Authors: Yuqi (Mark) Zhao and Claire Chen
*/

#include "Servo.h"

/*
* Configure ports PTD0 and PTD2 as the GPIO output pins for both servos.
*/
void servo_init(void){
	//Enable power to port D
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	//Set up each pin as GPIO output
	PORTD->PCR[SERVO1] |= PORT_PCR_MUX(001);
	PTD->PDDR |= (1 << SERVO1);
	PTD->PDOR &= ~(1 << SERVO1); //Initialize pin to 0
	PORTD->PCR[SERVO1] &= ~(1 << PORT_PCR_SRE_SHIFT); //Configure fast slew rate
	
	PORTD->PCR[SERVO2] |= PORT_PCR_MUX(001);
	PTD->PDDR |= (1 << SERVO2);
	PTD->PDOR &= ~(1 << SERVO2); //Initialize pin to 0
	PORTD->PCR[SERVO2] &= ~(1 << PORT_PCR_SRE_SHIFT); //Configure fast slew rate
}

/*
*	This function sets up PIT0 and PIT1, which will be used for setting the 
* Correct periods to control the servo positions!
*/
void servo_setup_timers(void){
	int default_position1;
	int default_position2;
	servo1_angle = SERVO1_NEUTRAL; 		//Initialize the start angle to 90
	servo2_angle = SERVO2_NEUTRAL;
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // Enable Clock to PIT
	PIT->MCR = 0x0; // Enables PIT timer, allows running in debug mode
	default_position1 = servo_get_high(servo1_angle);  // Sets both servos to 90 degrees
	default_position2 = servo_get_high(servo2_angle);
	
	// Set up PIT0 and PIT1
	PIT->CHANNEL[0].LDVAL = default_position1;
	PIT->CHANNEL[0].TCTRL = 3;
	PIT->CHANNEL[1].LDVAL = default_position2;
	PIT->CHANNEL[1].TCTRL = 3;
	
	//Enable interrupts on PIT0 and PIT1
	NVIC_EnableIRQ(PIT0_IRQn);
	NVIC_EnableIRQ(PIT1_IRQn);
}


/*
*	Takes in angle between 0 and 180 degrees
* Returns integer corresponding to appropriate high load value between 1ms and 2ms
*/
int servo_get_high(int angle){
	float angle_range = (float)(MAX_ANGLE - MIN_ANGLE);
	float angle_distance = angle/angle_range;
	int load_val = (int) (MILLISECOND + MILLISECOND * (angle_distance));
	return load_val;
}

/*
*	Takes in load value of high pulse
* Returns integer corresponding to appropriate low load value
*/
int servo_get_low(int angle){
	int high_load_val = servo_get_high(angle);
	int low_load_val = PERIOD - high_load_val;
	return low_load_val;
}

/*
* Signals servo to hit, depending on the servo_num
*/
void servo_hit(int servo_num){
		if(servo_num == 1){
			tap_dat_1 = 1; 
			//Set servo angle to hit
			servo1_angle = SERVO1_HIT;
			
			/*
			if(intflag == 0){
				intflag = 1;
				LEDGrn_On();
			}
			else{
				intflag = 0;
				LEDGrn_Off();
			}
			*/
		}
		else if(servo_num == 2){
			tap_dat_2 = 1;
			servo2_angle = SERVO2_HIT;
			/*
			if(intflag2 == 0){
				intflag2 = 1;
				LEDRed_On();
			}
			else{
				intflag2 = 0;
				LEDRed_Off();
			}
			*/
		}
}


