#include "Beats.h"
#include "LED.h"

void add_to_tail(beat_t** head_ref, beat_t* b){
	// Get point to head of list
	beat_t* current = *head_ref;
	
	// If queue is empty, set head to b
	if (current==NULL) { *head_ref = b; }
	
	// Otherwise, append beat to tail of beat list
	else{
		while (current->next != NULL) {current = current->next;}
		//When we've arrived at end of list, append beat
		current->next = b;
	}
}

/*
*	Function that is first called when reset. Waits for user to press button.
*/
void prompt_for_input(void){
	//Turn on the green LED showing that user should begin inputting beats
	LEDGrn_On();
}

/*
* Function called to setup up PIT2, used to keep track of duration between taps
* DO NOT start timer yet.
*/
void setup_beat_timer(void){
	PIT->CHANNEL[2].LDVAL = MILLISECOND;
	PIT->CHANNEL[2].TCTRL = 2; //Enable Interrupts, don't start timer.
	NVIC_EnableIRQ(PIT2_IRQn);
}

/*
* Function that mallocs space for each beat when a button is input by the user.
* Params: int servo - which servo is hit. int time - time in ms of press after period starts
* Return: A pointer to the beat_t in memory, no guarantees on if malloc failed or not
*/
beat_t* create_beat(int servo, int time){
	beat_t* new_beat = (beat_t*) malloc(sizeof(beat_t));
	//Will check if malloc failed later
	if(new_beat != NULL){
		new_beat->hit_time = time;
		new_beat->servo_num = servo;
		new_beat->next = NULL;
	}
	return new_beat;
	
}
