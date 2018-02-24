/*
	graphics.h
	Created on Feb 16, 2018 by Hugo Sandell
	Contains graphics data declarations
*/
#ifndef GRAPHICAS_H
#define GRAPHICAS_H

typedef struct {
	const unsigned width;
	const unsigned height;
	const unsigned* data; // top(MSB) to bottom(LSB), left(Word #0) to right(Word #(width-1))
} Sprite;

/* DATA */
extern unsigned car_data[4];
extern Sprite spr_car;
//Sprite spr_car;

#endif // GRAPHICS_H
