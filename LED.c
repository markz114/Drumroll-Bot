#include "LED.h"


/*
* Set up LEDs for testing purposes
*/
void setup_LED(void){
	//Set up Red LED
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[22] = PORT_PCR_MUX(001);
	PTB->PDDR |= (1 << 22);
	
	//Set up Green LED
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[26] = PORT_PCR_MUX(001);
	PTE->PDDR |= (1 << 26);
	
	//Turn off the Green and Red LEDs
	LEDRed_Off();
	LEDGrn_Off();
}

/** 
 * Turns the red LED on 
*/ 
void LEDRed_On(void){ 
 	PTB->PCOR |= (1 << 22); 
} 
 
 
 /**  
* Turns the red LED off 
*/ 
void LEDRed_Off(void){ 
	PTB->PSOR |= (1 << 22); 
} 
 /** 
* Turns the green LED on 
*/ 
void LEDGrn_On(void){ 
	PTE->PCOR |= (1 << 26); 
} 

 
/** 
* Turns the red LED off 
*/ 
void LEDGrn_Off(void){ 
	PTE->PSOR |= (1 << 26); 
} 
