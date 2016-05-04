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
	PORTD->PCR[SERVO1] &= ~(1 << PORT_PCR_SRE_SHIFT); //Configure fast slew rate
	
	PORTD->PCR[SERVO2] |= PORT_PCR_MUX(001);
	PTD->PDDR |= (1 << SERVO2);
	PORTD->PCR[SERVO2] &= ~(1 << PORT_PCR_SRE_SHIFT); //Configure fast slew rate
}

/*
*	This function sets up PIT0 and PIT1, which will be used for setting the 
* Correct periods to control the servo positions!
*/
void servo_setup_timers(void){
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // Enable Clock to PIT
	PIT->MCR = 0x0; // Enables PIT timer, allows running in debug mode
	int default_position = servo_get_high(90);  // Sets both servos to 90 degrees
	
	// Set up PIT0 and PIT1
	PIT->CHANNEL[0].LDVAL = default_position;
	PIT->CHANNEL[0].TCTRL = 3;
	PIT->CHANNEL[1].LDVAL = default_position;
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
int servo_get_low(int high_load_val){
	int low_load_val = PERIOD - high_load_val;
	return low_load_val;
}