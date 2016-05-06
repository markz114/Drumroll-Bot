/*************************************************************************
*		Yuqi (Mark) Zhao and Claire Chen
*		ECE 3140 Final Project - Drumroll-Bot
*		Cornell University - Spring 2016
*
*/

#include "Buttons.h"
#include "LED.h"
#include "Servo.h"

volatile int intflag = 0;
volatile int intflag2 = 0;
volatile int counter = 0;

volatile int is_servo1_high = 0; //0 if low, 1 if high 
int servo_pos = 0;

/*
*		Main Function
*/
int main(void){
	
	//Test pullup
	configure_buttons();
	setup_LED();

	servo_init();
	servo_setup_timers();
	while(1);
	
	return 0;
}



/*
*	Interrupt Handler for Buttons
*/
void PORTC_IRQHandler(void)
{
	if (PORTC-> ISFR & (1<<BT1)){
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
		
	counter++;
	//Reset the ISF
	PORTC->PCR[BT1] |= (1 << 24);
	PORTC->PCR[BT2] |= (1 << 24);
	
}

/*
*	Interrupt Handler for servo 1
*/
void PIT0_IRQHandler(void){
	int val;
	counter++;
	PIT->CHANNEL[0].TCTRL = 0; //Disable Timer
	switch(is_servo1_high){
		
		case 0:
			//Load high timer value
			val = servo_get_high(servo_pos);
			
			//Write high
			PTD->PDOR |= (1 << SERVO1); //Write a high value
			is_servo1_high = 1;
		
			break;
		case 1:
			val = servo_get_low(servo_pos);
			//Write low
			PTD->PDOR &= ~(1 << SERVO1);
			is_servo1_high = 0;
			break;
	}
		
	PIT->CHANNEL[0].LDVAL = val; //Write appropriate timer val
	PIT->CHANNEL[0].TFLG |= (1 << 0); //Clear flag
	PIT->CHANNEL[0].TCTRL = 3; //Re-enable timer
	return;
}

