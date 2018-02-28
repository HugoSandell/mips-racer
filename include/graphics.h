/*
	graphics.h
	Created on Feb 16, 2018 by Hugo Sandell
	Contains graphics data declarations
*/
#include <stdint.h>
#ifndef GRAPHICS_H
#define GRAPHICS_H

typedef struct {
	const unsigned width;
	const unsigned height;
	const unsigned* data; // top(MSB) to bottom(LSB), left(Word #0) to right(Word #(width-1))
} Sprite;

/* DATA */
extern const unsigned const car_data[4];
extern const Sprite spr_car;

// FONT
extern const uint8_t const font[];

#endif // GRAPHICS_H
