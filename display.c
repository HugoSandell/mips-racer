/*
	display.c
	Created on Feb 16, 2018 by Hugo Sandell

	Provides helper functions for interacting with the display, as well as a buffer
	for
*/
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "graphics.h"
#include "chipkitio.h"

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)
extern int x;
unsigned char pixelbuffer[128][4]; // A buffer to store the data to be sent to the display

// This code was copied from the lab
uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

// Sets a single pixel with the coordinates (x, y)
void set_pixel(int x, int y) {
	// Ensure the specified pixel is in the range of the screen
	if(x < 0 || y < 0 || x > 127 || y > 31) return;
	// Set the correct bit
	pixelbuffer[x][y/8] |= 1 << y % 8;
}

// Clears a single pixel with the coordinates (x, y)
void clear_pixel(int x, int y) {
	// Ensure the specified pixel is in the range of the screen
	if(x < 0 || y < 0 || x > 127 || y > 31) return;
	// Clear the correct bit
	pixelbuffer[x][y/8] &= ~(1 << y % 8);
}

// Draws a sprite with top left corner at specified coordinates
void draw_sprite(int x, int y, const Sprite s) {
	unsigned i, j, word, bit, bit_in_word;
	for(i = 0; i < s.width; i++) {
		for(j = 0; j < s.height; j++) {
			bit = (i*s.height+j);
			word = bit/32;
			bit_in_word = bit%32;
			if(s.data[word] & (1 << bit_in_word))
				set_pixel(x+i, y+j);
		}
	}
}

void fill_rect(int x1, int y1, int x2, int y2){
  unsigned i, j;
	for(i = x1; i <= x2; i++)
		for(j = y1; j <= y2; j++)
      set_pixel(i, j);
}

// Clears the whole buffer
void clear_buffer(void) {
	int i,j;
	for(i = 0; i < 128; i++){
		for(j = 0; j < 4; j++)
			pixelbuffer[i][j]=0;
	}
}


// Starts up the display
void display_init(void) {
  DISPLAY_CHANGE_TO_COMMAND_MODE;
	idle(10);
	DISPLAY_ACTIVATE_VDD;
	idle(1000000);

	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	idle(10);
	DISPLAY_DO_NOT_RESET;
	idle(10);

	spi_send_recv(0x8D);
	spi_send_recv(0x14);

	spi_send_recv(0xD9);
	spi_send_recv(0xF1);

	DISPLAY_ACTIVATE_VBAT;
	idle(10000000);

	spi_send_recv(0xA1);
	spi_send_recv(0xC8);

	spi_send_recv(0xDA);
	spi_send_recv(0x20);

	spi_send_recv(0xAF);
}

// Send the buffer to the display
// Code copied from lab
void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) { //i corresponds to line
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 16; j++) { // J corresponds to character on line
			for(k = 0; k < 8; k++) // k corresponds to column in character
				spi_send_recv(pixelbuffer[j*8+k][i]);
		}
	}
	clear_buffer();
}
