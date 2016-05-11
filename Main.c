/*************************************************************************
*		Yuqi (Mark) Zhao and Claire Chen
*		ECE 3140 Final Project - Drumroll-Bot
*		Cornell University - Spring 2016
*
*/

#include "Buttons.h"
#include "LED.h"
#include "Servo.h"
#include "Beats.h"

//Flags that store if the red or green LED are on
volatile int intflag = 0;
volatile int intflag2 = 0;
//volatile int counter = 0;

//Servo 1 
volatile int is_servo1_high = 0; //0 if low, 1 if high 
volatile int servo1_angle = 0;   //Global variable to hold angle of servo1
volatile int tap_dat_1 = 0; 		 //Flag that is set when we want to "hit" the first drum
volatile int tap_counter_1 = 0;  //Counter that holds time elapsed for a "hit" of servo 1

//Servo 2
volatile int is_servo2_high = 0; //0 if low, 1 if high 
volatile int servo2_angle = 0;   //Global variable to hold angle of servo2
volatile int tap_dat_2 = 0; 		 //Flag that is set when we want to "hit" the first drum
volatile int tap_counter_2 = 0;  //Counter that holds time elapsed for a "hit" of servo 2

//Head of list that stores user-entered beats
beat_t* beat_list = NULL;

//Counter that keeps track of time elapsed per period
volatile int time_ms = 0;
//volatile int start_time = 0;

//State variable
volatile int state = 0;

/*
*		Main Function
*/
int main(void){
	
	/**********************Configure Timers, Buttons, Servos********************************/
	//Test pullup
	configure_buttons();
	setup_LED();

	servo_init();
	servo_setup_timers();
	
	//Set up the current_time timer
	setup_beat_timer();
	
	/****************************************************************************************/
	
	
	//Now prompt for user input
	prompt_for_input();
	
	//Do nothing until we're in state 3. Then delay and repeat the sequence
	while(state < 2){
		//Spin and wait for users to finish inputting data
	}
	
	delay(DELAY_TIME);
	LEDRed_Off();
	state = 3;
	
	//Now keep walking through the beat list and replay each beat
	while(1){
		
		LEDGrn_On();
		delay(1000000);
		LEDGrn_Off();
		delay(1000000);
	};
	
	return 0;
}

/***************************Button IRQ Handlers**********************************/

/*
*	Interrupt Handler for Buttons
*/
void PORTC_IRQHandler(void){
	if( (state == 0 || state == 1) && ( (PORTC->ISFR & (1<<BT1)) || (PORTC->ISFR & (1<<BT2)) )){
		beat_t* beat;
		
		// This is the first beat, so append to head
		if(PORTC->ISFR & (1<<BT1)){
			beat = create_beat(1, time_ms);
		}
		else{
			beat = create_beat(2, time_ms);
		}
		
		//Append to beat list
		if(beat != NULL){
			add_to_tail(&beat_list, beat);
		}
		
		if(state == 0){
			state = 1;
			//Start the timer
			PIT->CHANNEL[2].TCTRL = 3;
			LEDRed_On();
		}
	}
	
	//END command is hit
	if( (state == 1) && (PORTC->ISFR & (1<<BT3)) )
	{
		LEDGrn_Off();
		state = 2;
		//Stop the timer and reset the time
		PIT->CHANNEL[2].TCTRL = 0;
		time_ms = 0;
	}
	
	//Functions to hit servos and stuff
	//NVIC_DisableIRQ(PORTC_IRQn);
	//delay(500000);
	if (PORTC-> ISFR & (1<<BT1) && (state == 0 || state == 1)){
		tap_dat_1 = 1; 
		//Set servo angle to hit
		servo1_angle = SERVO_HIT;
		
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
	else if(PORTC->ISFR & (1<<BT2) && (state == 0 || state == 1)){
		tap_dat_2 = 1;
		//Set servo angle to hit
		servo2_angle = SERVO_HIT;
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
	//NVIC_EnableIRQ(PORTC_IRQn);
	//Reset the ISF
	PORTC->PCR[BT1] |= (1 << 24);
	PORTC->PCR[BT2] |= (1 << 24);
	PORTC->PCR[BT3] |= (1 << 24);
}

/****************************************************************************************/
/**********************PIT IRQ Handlers ***********************************************/
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

/*
*	Interrupt Handler for servo 1. This should trigger twice every 20ms.
*/
void PIT1_IRQHandler(void){
	int val;
	PIT->CHANNEL[1].TCTRL = 0; //Disable Timer
	switch(is_servo2_high){
		
		case 0:
			//Load high timer value
			val = servo_get_high(servo2_angle);
			
			//Write high
			PTD->PDOR |= (1 << SERVO2); //Write a high value
			is_servo2_high = 1;
			
			//Increase the tap_counter_2 every 20ms only when we're tapping
			if(tap_dat_2){
				tap_counter_2++;
			}
		
			break;
		case 1:
			val = servo_get_low(servo2_angle);
			//Write low
			PTD->PDOR &= ~(1 << SERVO2);
			is_servo2_high = 0;
			break;
	}
	//If we've counted 20x (400ms have elapsed), then raise servo to neutral pos
	if(tap_counter_2 >= TAP_LIMIT){
		tap_dat_2 = 0; //no more tapping :(
		servo2_angle = SERVO_NEUTRAL;
		tap_counter_2 = 0; //Reset Counter for future tapping
	}
		
	PIT->CHANNEL[1].LDVAL = val; //Write appropriate timer val
	PIT->CHANNEL[1].TFLG |= (1 << 0); //Clear flag
	PIT->CHANNEL[1].TCTRL = 3; //Re-enable timer
	return;
}

/*
* IRQ Handler to increment the global time variable for each period.
*/
void PIT2_IRQHandler(void){
	time_ms++;
	
	//Reset the interrupt flag and refresh the timer.
	PIT->CHANNEL[2].TFLG = 1;
	PIT->CHANNEL[2].TCTRL = 0;
	PIT->CHANNEL[2].TCTRL = 3;
	//else do nothing
}
/******************************************************************************/

/*********************** Helper Functions****************************************/
void delay(int delay){
	int i;
	for(i = 0; i < delay; i++);
}
/******************************************************************************/

