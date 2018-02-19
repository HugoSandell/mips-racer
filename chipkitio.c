/*	
	chipkitio.c
	Created on Feb 16, 2018 by Hugo Sandell

	Contains low level code to interact with I/O devices
*/
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipkitio.h"

// Get the state of the I/O shield push buttons
unsigned getButtons() {
	return ((PORTD & (7 << 5)) >> 4) | ((PORTF & 2) >> 1);
}

// Get the state of the I/O shield switches
unsigned getSwitches() {
	return (PORTD & (0xF << 8)) >> 8;
}

// Get the state of an I/O shield push button n
unsigned getButton(int n) {
	if(n == BTN1)
		return (PORTF & 2) >> 1;
	return (PORTD >> (4 + n)) & 1;
}

// Get the state of an I/O shield switch n
unsigned getSwitch(int n) {
	return (PORTD >> (8 + n)) & 1;
}

// This code was copied from the lab
void setupDisplayPins(void) {
	/* Output pins for display signals */
	PORTE=0;
	TRISECLR=0xFF;
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
}

// Configure I/O shield user inputs
void setupInputs(void) {

	TRISDSET = 0x7F << 5; // Button 1 and switches
	TRISFSET = 2; // Buttons 2-4

	AD1PCFG = 0xFFFB;
	AD1CON1 = 0x00E0;
	AD1CHS = 0x00020000;
	AD1CSSL = 0;
	AD1CON3 = 0x0203;
	AD1CON2 = 0x6004;
	AD1CON1SET = 1<<15;
}

// This code was copied from the lab
void setupDisplaySPI(void) {
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
    SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;
}

// This code was copied from the lab 
void synchronizeClocks(void) {
	/*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
}