#include "car.h"
#include "graphics.h"
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

extern volatile unsigned game_time;

void initialize_car(Car* car) {
 car->alive = 0;
 car->progress = 0;
 car->side = 0;
}

void start_car(Car* car, int side) {
 car->alive = 1;
 car->progress = 0;
 car->side = side;
}

void update_car(Car* car, float v) {
 if(car->alive == 0) return;
 car->progress += v;
 if (car->progress >= 128+16) car->alive = 0;
}

void draw_car(Car* car) {
 if(car->alive == 0) return;
 draw_sprite((int)car->progress - 16, car->side*24, spr_car);
}
