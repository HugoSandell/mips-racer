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

static unsigned char pixelbuffer[128][4]; // A buffer to store the data to be sent to the display

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
void draw_sprite(int x, int y, const Sprite s, unsigned flags) {
	unsigned i, j, word, bit, bit_in_word, px, py;
	for(i = 0; i < s.width; i++) {
		for(j = 0; j < s.height; j++) {
			bit = (i*s.height+j);
			word = bit/32;
			bit_in_word = bit%32;
			if(s.data[word] & (1 << bit_in_word)) {
        px = x;
        py = y;
        if(flags & MIRROR_VERTICAL){
          py += s.height - j - 1;
        }else{
          py += j;
        }
        if(flags & MIRROR_HORIZONTAL){
          px += s.width - i - 1;
        }else{
          px += i;
        }
				set_pixel(px, py);
      }
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
  PORTFCLR = 0x10;
	idle(10);
	PORTFCLR = 0x40;
	idle(100000);

	spi_send_recv(0xAE);
	PORTGCLR = 0x200;
	idle(10);
	PORTGSET = 0x200;
	idle(10);

	spi_send_recv(0x8D);
	spi_send_recv(0x14);

	spi_send_recv(0xD9);
	spi_send_recv(0xF1);

	PORTFCLR = 0x20;
	idle(1000000);

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
		PORTFCLR = 0x10;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		PORTFSET = 0x10;

		for(j = 0; j < 16; j++) { // J corresponds to character on line
			for(k = 0; k < 8; k++) // k corresponds to column in character
				spi_send_recv(pixelbuffer[j*8+k][i]);
		}
	}
	clear_buffer();
}
