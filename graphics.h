/*
	graphics.h
	Created on Feb 16, 2018 by Hugo Sandell
	Contains graphics data
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

typedef struct {
	const unsigned width;
	const unsigned height;
	const unsigned* data; // top(MSB) to bottom(LSB), left(Word #0) to right(Word #(width-1))
} Sprite;

unsigned data[] = {0xFF7E3C00, 0x7E7EFFFF, 0xFF7E7E7E, 0x007EFFFF};
Sprite car = {.width = 16, .height = 8, .data = data};

#endif // GRAPHICS_H