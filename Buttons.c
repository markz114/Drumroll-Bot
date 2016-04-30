/*
*
*		Buttons.c
*
*		Created: April 30 2016
*		Authors: Yuqi (Mark) Zhao and Claire Chen
*/

#include "Buttons.h"

/*Configures the pins for the buttons
* We use PTC{9,0,7,5} corresponding to each button on the keypad
* Pullup each pin to logic 1 - Check for presses interrupts on falling edge
*/
void configure_buttons(void){
	//Enable power to ports
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	//Set up each pin as GPIO output
	PORTC->PCR[3] |= PORT_PCR_MUX(001);
	PTC->PDDR |= (0 << 3);
	
	//Pullup each pin to logic 1
	PORTC->PCR[3] |= (1 << 1);
	PORTC->PCR[3] |= 1;
}
