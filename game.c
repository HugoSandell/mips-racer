/*
	game.c
	Created on Feb 16, 2018 by Hugo Sandell
*/

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdbool.h>  /* Declarations of system-specific addresses etc */
#include "chipkitio.h"
#include "graphics.h"
#include "car.h"

#define FRAMES_PER_SPEEDUP 2000 // How many frames before speed increases
#define SPEED_SCALE 20 // Scalar for speed (divisor)
#define TICKS_PER_CAR 20 // How many ticks minimum before spawning new car
#define MAX_CARS 4 // How many maximum cars can be on the road at one time

/* GAME STATE */
int car_pos; // the position of the car, on a range from 0 to 24
extern long long game_time; // how many ticks have passed since start of game
int speed; // the speed of the car
Car* cars[MAX_CARS]; // other cars on the road

// initialize or reset the game
void init_game(void) {
  int i;
  car_pos = 16-4;
  speed = SPEED_SCALE;
  for (i=0; i < MAX_CARS; i++) {
    Car c = {.alive = 0, .side = 0, .progress = 0};
    cars[i] = &c;
  }
}

// Make sure car stays within bounds
void bound_car() {
  if (car_pos < 0) car_pos = 0;
    if (car_pos > 32-8) car_pos = 32-8;
}

// Draw the road in the background
void draw_road(){
  int i;
  int road_mod = ((game_time*speed)/SPEED_SCALE) % (128/5);
  //PORTECLR = 0xFF;
  //PORTESET = 0xC0 >> (road_mod)/4; // LED effects
  for(i = 0; i < 6; i++)
    fill_rect(i * (128/5) + road_mod - 6 , 15,
    i * (128/5) + road_mod, 16);
}

// Determine if a car should be spawned
int should_spawn_car(){
  if(game_time % TICKS_PER_CAR != 0) return 0; // Only spawn in intervals
  if(rand()%2 == 0) return 0; // Random chance
  return 1; // Should spawn
}

// Add new car to game
void spawn_car() {
  int i;
  for (i=0; i < MAX_CARS; i++){
    if(cars[i]->alive != 1) {
      initialize_car(cars[i], rand()%2);
      break;
    }
  }
}

bool has_crashed(){
  int i;
  for (i = 0; i < MAX_CARS; i++){
    if(cars[i]->alive == 0) continue;
    if (cars[i]->progress <= 112) continue;
    if (cars[i]->progress >= 128) continue;
    if (cars[i]->side == 0 && car_pos > 8) continue;
    if (car_pos < 16) continue;
    return true;
  }
  return false;
}

void draw() {
  int i;
  draw_road();
  draw_sprite(127 - 16, car_pos, spr_car);

  for (i=0; i < MAX_CARS; i++) {
      draw_car(cars[i]);
  }
}

void tick(void) {
  int i;
  if(game_time % FRAMES_PER_SPEEDUP == 0) speed++;

  if(should_spawn_car()) spawn_car();

  car_pos += get_button(BTN1) - get_button(BTN2);
  bound_car();

  for (i=0; i < 4; i++) {
      update_car(cars[i], (speed+CAR_SPEED)/SPEED_SCALE);
  }

  if(has_crashed())PORTE = (PORTE + 1) & 0xFF;

  draw();
	display_update();
}
