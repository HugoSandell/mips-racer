/*
	graphics.h
	Created on Feb 16, 2018 by Hugo Sandell
	Contains graphics data declarations
*/
#include <stdint.h>
#ifndef GRAPHICS_H
#define GRAPHICS_H

// Sprite drawing flags //
#define MIRROR_HORIZONTAL 1
#define MIRROR_VERTICAL 2

typedef struct {
	const unsigned width;
	const unsigned height;
	const unsigned* data; // top(MSB) to bottom(LSB), left(Word #0) to right(Word #(width-1))
} Sprite;

/* DATA */
extern const Sprite spr_car;

// FONT
extern const uint8_t const font[];

#endif // GRAPHICS_H
