/*************************************************************************
*		Yuqi (Mark) Zhao and Claire Chen
*		ECE 3140 Final Project - Drumroll-Bot
*		Cornell University - Spring 2016
*
*/

#include "Buttons.h"
#include "LED.h"

volatile int intflag = 0;
volatile int intflag2 = 0;
volatile int counter = 0;

/*
*		Main Function
*/
int main(void){
	
	//Test pullup
	configure_buttons();
	setup_LED();

	
	while(1);
	
	return 0;
}



/*
*	Intterrupt Handler for Buttons
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

