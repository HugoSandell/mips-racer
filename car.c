#include "car.h"
#include "graphics.h"
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

void initialize_car(Car* car, int side) {
 car->alive = 1;
 car->progress = 0;
 car->side = side;
}

void update_car(Car* car, int v) {
 if(car->alive == 0) return;
 car->progress += v;
 if (car->progress >= 128) car->alive = 0;
}

void draw_car(Car* car) {
 if(car->alive == 0) return;
 draw_sprite(car->progress - 16, car->side*24, spr_car);
}
