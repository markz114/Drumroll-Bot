/*************************************************************************
*		Yuqi (Mark) Zhao and Claire Chen
*		ECE 3140 Final Project - Drumroll-Bot
*		Cornell University - Spring 2016
*
*/

#include "Buttons.h"
#include "LED.h"
#include "Servo.h"
#include <stdlib.h>

volatile int intflag = 0;
volatile int intflag2 = 0;
volatile int counter = 0;

volatile int is_servo1_high = 0; //0 if low, 1 if high 
volatile int servo1_angle = 0;   //Global variable to hold angle of servo1
volatile int tap_dat_1 = 0; 		 //Flag that is set when we want to "hit" the first drum
volatile int tap_counter_1 = 0;  //Counter that holds time elapsed for a "hit" of servo 1

/*
*		Main Function
*/
int main(void){
	
	//Test pullup
	configure_buttons();
	setup_LED();

	servo_init();
	servo_setup_timers();
	
	while(1){
	};
	
	return 0;
}



/*
*	Interrupt Handler for Buttons
*/
void PORTC_IRQHandler(void)
{
	if (PORTC-> ISFR & (1<<BT1)){
		tap_dat_1 = 1; 
		//Set servo angle to hit
		servo1_angle = SERVO_HIT;
		
		
		if(intflag == 0){
			intflag = 1;
			LEDGrn_On();
		}
		else{
			intflag = 0;
			LEDGrn_Off();
		}
	}
	else if(PORTC->ISFR & (1<<BT2)){
		if(intflag2 == 0){
			intflag2 = 1;
			LEDRed_On();
		}
		else{
			intflag2 = 0;
			LEDRed_Off();
		}
	}
		
	//Reset the ISF
	PORTC->PCR[BT1] |= (1 << 24);
	PORTC->PCR[BT2] |= (1 << 24);
	
}

/*
*	Interrupt Handler for servo 1. This should trigger twice every 20ms.
*/
void PIT0_IRQHandler(void){
	int val;
	PIT->CHANNEL[0].TCTRL = 0; //Disable Timer
	switch(is_servo1_high){
		
		case 0:
			//Load high timer value
			val = servo_get_high(servo1_angle);
			
			//Write high
			PTD->PDOR |= (1 << SERVO1); //Write a high value
			is_servo1_high = 1;
			
			//Increase the tap_counter_1 every 20ms only when we're tapping
			if(tap_dat_1){
				tap_counter_1++;
			}
		
			break;
		case 1:
			val = servo_get_low(servo1_angle);
			//Write low
			PTD->PDOR &= ~(1 << SERVO1);
			is_servo1_high = 0;
			break;
	}
	//If we've counted 20x (400ms have elapsed), then raise servo to neutral pos
	if(tap_counter_1 >= TAP_LIMIT){
		tap_dat_1 = 0; //no more tapping :(
		servo1_angle = SERVO_NEUTRAL;
		tap_counter_1 = 0; //Reset Counter for future tapping
	}
		
	PIT->CHANNEL[0].LDVAL = val; //Write appropriate timer val
	PIT->CHANNEL[0].TFLG |= (1 << 0); //Clear flag
	PIT->CHANNEL[0].TCTRL = 3; //Re-enable timer
	return;
}

void delay(void){
	int i;
	for(i = 0; i < 500000; i++){
		//blah
	}
}
