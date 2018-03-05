/*
	chipkitio.c
	Created on Feb 16, 2018 by Hugo Sandell

	Contains low level code to interact with I/O devices
*/
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipkitio.h"

// Get the state of the I/O shield push buttons
unsigned get_buttons() {
	return ((PORTD & (7 << 5)) >> 4) | ((PORTF & 2) >> 1);
}

// Get the state of the I/O shield switches
unsigned get_switches() {
	return (PORTD & (0xF << 8)) >> 8;
}

// Get the state of an I/O shield push button n (0-3)
unsigned get_button(int n) {
	if(n == BTN1)
		return (PORTF & 2) >> 1;
	return (PORTD >> (4 + n)) & 1;
}

// Get the state of an I/O shield switch n
unsigned get_switch(int n) {
	return (PORTD >> (8 + n)) & 1;
}

// Timer 3 for game ticks
void init_game_timer(void) {
  disable_interrupts();
  T3CON = 7 << 4;
  TMR3 = 0;
  PR3 = 217; // 1/1440 seconds with 256 prescaling
  IECSET(0) = 1<<12;
  IFSCLR(0) = 1<<12;
  IPCCLR(3) = 0x1f; // Clear interrupt priority bytes of Timer 3
  IPCSET(3) = 0x1f; // Set interrupt priority bytes of Timer 3
  T3CONSET = 0x8000;
}

void setup_display_pins(void) {
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
void setup_inputs(void) {
	TRISDSET = 0x7F << 5; // Button 1 and switches
	TRISFSET = 2; // Buttons 2-4
}

// Set up spi as master
void setup_display_spi(void) {
	SPI2CON = 0;
	SPI2BRG = 4; // Baud rate
	SPI2STATCLR = 0x40; // Clear recieve overflow flag
  SPI2CONSET = 0x40;
	SPI2CONSET = 0x20;
	SPI2CONSET = 0x8000;
}

// This code was copied from the lab
void synchronize_clocks(void) {
	/*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;
	SYSKEY = 0x556699AA; 
	while(OSCCON & (1 << 21));
	OSCCONCLR = 0x180000;
	while(OSCCON & (1 << 21));
	SYSKEY = 0x0;
}
