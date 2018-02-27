/*
  game.c
  Created on Feb 16, 2018 by Hugo Sandell
*/

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdbool.h>  /* Standard header for dealing with boolean values */
#include <math.h>
#include "chipkitio.h"
#include "graphics.h"
#include "car.h"

#define ACCELERATION 4 // Increase in speed per frame
#define ROAD_LINE_SPACE (16) // Distance between lines on the road
#define START_SPEED 100 // Start speed value
#define BASE_CAR_SPACING 80.0 // Minimum space between spawning cars
#define CAR_RATE 80 // Chance in percent of spawning car at intervals determined by TICKS_PER_CAR
#define MAX_CARS 8 // How many maximum cars can be on the road at one time
#define PLAYER_SPEED 200.0 // How fast does the player's car turn left or right?
#define SPEED_SCALE 1000.0 // Universal scalar for speeds (divisor)
#define BALANCE_FACTOR 10.0

/* GAME STATE */
float car_pos; // the position of the car, on a range from 0 to 24
extern volatile unsigned game_time; // how many ticks have passed since start of game
float speed; // the speed of the car
Car cars[MAX_CARS]; // other cars on the road
float road_anim; // road animation variable
int debug_var; // variable for debugging
int balance = 0; // variable for balancing car spawning
int spawn_timer;

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
  int line_offset; // The offset of a particular line
  for(i = 0; i < 128/ROAD_LINE_SPACE+1; i++) {
    line_offset = (int)(i * ROAD_LINE_SPACE + road_anim);
    fill_rect(line_offset - 6, 15, line_offset, 16);
  }
}

// Determine if a car should be spawned
static int should_spawn_car(){
  if (spawn_timer++ < SPEED_SCALE * BASE_CAR_SPACING/speed)
    return 0; // Only spawn in intervals
  spawn_timer = 0;
  if(rand()%100 >= CAR_RATE)
    return 0; // Random chance
  return 1; // Should spawn
}

// Add new car to game
static void spawn_car() {
  int i;
  PORTEINV=0xFF;
  for (i=0; i < MAX_CARS; i++){
    if(cars[i].alive != 1) {
      int side = (rand()%100 + balance) < 50;
      balance += (side*2-1) * BALANCE_FACTOR;
      start_car(&cars[i], side);
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
  draw_sprite(127 - 16, (int)car_pos, spr_car); //Draw player car

  // LED Effect
  /*int led_stripe = 0x300 >> (int)(32*road_anim/ROAD_LINE_SPACE);
  PORTECLR = 0xFF;
  PORTESET = led_stripe & 0xFF;
  */
  //Draw cars
  for (i=0; i < MAX_CARS; i++) {
      draw_car(&cars[i]);
  }
}

void tick(void) {
  // Increase speed
  speed += ACCELERATION/SPEED_SCALE;

  //Spawning cars
  if(should_spawn_car())
    spawn_car();

  //Player controlled car
  float turning = get_button(BTN1) - get_button(BTN2);
  turning *= (PLAYER_SPEED/SPEED_SCALE);
  car_pos += turning;
  bound_car();
  debug_var = (int)(SPEED_SCALE);

  //Road animation
  road_anim += speed/SPEED_SCALE;
  while(road_anim >= ROAD_LINE_SPACE)
    road_anim -= ROAD_LINE_SPACE;

  //Cars
  int i;
  for (i=0; i < MAX_CARS; i++) {
      update_car(&cars[i], (speed + CAR_SPEED) / SPEED_SCALE);
  }
}
