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
	PORTC->PCR[BT2] |= PORT_PCR_MUX(001);
	PTC->PDDR |= (0 << BT2);
	
	PORTC->PCR[BT1] |= PORT_PCR_MUX(001);
	PTC->PDDR |= (0 << BT1);
	
	//Pulldown each pin to logic 0
	PORTC->PCR[BT2] |= (1 << 1);
	PORTC->PCR[BT2] |= 0;	
	PORTC->PCR[BT1] |= (1 << 1);
	PORTC->PCR[BT1] |= 0;
	
	
	//Enable Interrupts on Rising
	PORTC->PCR[BT2] |= PORT_PCR_IRQC(1001);
	PORTC->PCR[BT2] |= (1 << 24);
	
	PORTC->PCR[BT1] |= PORT_PCR_IRQC(1001);
	PORTC->PCR[BT1] |= (1 << 24);
	
	//Enable PortC's IRQ Handler
	NVIC_EnableIRQ(PORTC_IRQn);
}


