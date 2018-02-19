/*	
	main.c
	Created on Feb 16, 2018 by Hugo Sandell
	
	The entry point of the program
*/
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipkitio.h"

void idle(int cyc){
	int i;
	for(i = 0; i < cyc; i++);
}

void enable_interrupts(){
	__asm__("ei");
}

void disable_interrupts(){
	__asm__("di");
}

void user_isr() {

}

int main() {
	int i;
	volatile unsigned* ADC16Ptr;
	unsigned ADCValue;
	synchronizeClocks();
	setupDisplayPins();
	setupDisplaySPI();
	display_init();
	idle(60000);
	setupInputs();
	enable_interrupts();
	while(1){
		ADCValue = 0;
		ADC16Ptr = &ADC1BUF0;
		IFSCLR(1) = 2;
		AD1CON1SET = 4;
		while (!IFS(1) & 2);
			AD1CON1CLR = 4;
		for (i = 0; i < 2; i++)
		{
			ADCValue = ADCValue + *ADC16Ptr++;
			ADCValue = ADCValue >> 1;
		}
		PORTE = ADC1BUF0 >> 8;

		display_update();
		idle(60000);
	}

	return 0;
}
