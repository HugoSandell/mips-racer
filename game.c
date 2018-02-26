/*
	game.c
	Created on Feb 16, 2018 by Hugo Sandell
*/

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdbool.h>  /* Standard header for dealing with boolean values */
#include "chipkitio.h"
#include "graphics.h"
#include "car.h"

#define FRAMES_PER_SPEEDUP 500 // How many frames before speed increases
#define ROAD_LINE_SPACE (16) // Distance between lines on the road
#define START_SPEED 200 // Start speed value
#define SPEED_SCALE 2000.0 // Scalar for speed (divisor)
#define TICKS_PER_CAR 70 // How many ticks minimum before spawning new car
#define CAR_RATE 20 // Chance in percent of spawning car at intervals determined by TICKS_PER_CAR
#define MAX_CARS 8 // How many maximum cars can be on the road at one time

/* GAME STATE */
int car_pos; // the position of the car, on a range from 0 to 24
extern volatile unsigned game_time; // how many ticks have passed since start of game
int speed; // the speed of the car
Car cars[MAX_CARS]; // other cars on the road
int road_anim; // road animation variable
int debug_var; // variable for debugging

// initialize or reset the game
void init_game(void) {
  int i;
  road_anim = 0;
  car_pos = 16-4;
  speed = START_SPEED;
  for(i = 0; i < MAX_CARS; i++)
    initialize_car(&cars[i]);
  return;
}

// Make sure car stays within bounds
static void bound_car() {
  if (car_pos < 0) car_pos = 0;
    if (car_pos > 32-8) car_pos = 32-8;
  return;
}

// Draw the road in the background
static void draw_road(){
  int i;
  //PORTECLR = 0xFF;
  //PORTESET = 0xC0 >> (road_anim)/4; // LED effects
  for(i = 0; i < 6; i++)
    fill_rect((i * ROAD_LINE_SPACE) + road_anim - 6 , 15,
    (int)(i * ROAD_LINE_SPACE) + road_anim, 16);
}

// Determine if a car should be spawned
static int should_spawn_car(){
  if(game_time % TICKS_PER_CAR != 0) return 0; // Only spawn in intervals
  if(rand()%100 > CAR_RATE) return 0; // Random chance
  return 1; // Should spawn
}

// Add new car to game
static void spawn_car() {
  int i;
  for (i=0; i < MAX_CARS; i++){
    if(cars[i].alive != 1) {
      start_car(&cars[i], rand()%2);
      break;
    }
  }
}

static bool has_crashed(){
  int i;
  for (i = 0; i < MAX_CARS; i++){
    if(cars[i].alive == 0) continue; // NPCar must be alive
    if (cars[i].progress <= 112) continue; // NPCar can't be too far in front
    if (cars[i].progress >= 128) continue; // NPCar can't be too far behind
    if (cars[i].side == 0){
      if(car_pos > 12) continue; // Car is on right side
    } else if (car_pos < 11) continue; // Car is on left side
    return true; // Collision
  }
  return false; // No collision
}

void draw() {
  int i;
  draw_road();
  draw_sprite(127 - 16, car_pos, spr_car); //Draw player car
  PORTE ^= (int)road_anim & 0xFF; //### TODO: Something stops the increase of road_Anim and other things after a short amount of time after launch

  //Draw cars
  for (i=0; i < MAX_CARS; i++) {
      draw_car(&cars[i]);
  }
}

void tick(void) {
  int i;
  // Increase speed
  if(game_time % FRAMES_PER_SPEEDUP == 0) speed++;

  //Spawning cars
  if(should_spawn_car()) spawn_car();

  //Player controlled car
  car_pos += get_button(BTN1) - get_button(BTN2);
  bound_car();
  debug_var = (int)ceil(SPEED_SCALE/speed);
  //Road animation
  road_anim +=game_time%(int)ceil(SPEED_SCALE/speed) == 0;
  if(road_anim >= ROAD_LINE_SPACE) road_anim -= ROAD_LINE_SPACE;

  //Cars
  for (i=0; i < MAX_CARS; i++) {
      update_car(&cars[i], (game_time+CAR_SPEED)%(int)ceil(SPEED_SCALE/speed) == 0);
  }
}
