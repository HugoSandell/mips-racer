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
#include "states.h"
#include "scoreboard.h"
#include "globals.h"

#define ACCELERATION 20 // Increase in speed
#define ROAD_LINE_SPACE (16) // Distance between lines on the road
#define START_SPEED 500 // Start speed value
#define TICKS_PER_CAR 0.1 // How many ticks minimum before spawning new car
#define CAR_RATE 40 // Chance in percent of spawning car at intervals determined by TICKS_PER_CAR
#define MAX_CARS 8 // How many maximum cars can be on the road at one time
#define PLAYER_SPEED 400.0 // How fast does the player's car turn left or right?
#define TURN_ACCELERATION 0.5f; // Increase in turn speed
#define SPEED_SCALE 1000.0 // Universal scalar for speeds (divisor)
#define BALANCE_FACTOR 5.0 // Constant for tweaking the weight of the spawn balancing

/* GAME STATE */
float car_pos; // the position of the car, on a range from 0 to 24
extern volatile unsigned game_time; // how many ticks have passed since start of game
float speed; // the speed of the car
Car cars[MAX_CARS]; // other cars on the road
float road_anim; // road animation variable
int debug_var; // variable for debugging
int balance; // variable for balancing car spawning
static int score = 0;
static bool start = false;

// initialize or reset the game
void init_game(void) {
  int i;
  start = false;
  score = 0;
  road_anim = 0;
  car_pos = 16-4;
  speed = START_SPEED;
  balance = 0;
  for(i = 0; i < MAX_CARS; i++)
    initialize_car(&cars[i]);
  return;
}

// Make sure car stays within bounds
static void bound_car(void) {
  if (car_pos < 0) car_pos = 0;
    if (car_pos > 32-8) car_pos = 32-8;
  return;
}

// Draw the road in the background
static void draw_road(void){
  int i;
  for(i = 0; i < 128/ROAD_LINE_SPACE+1; i++)
    fill_rect((int)(i * ROAD_LINE_SPACE + road_anim) - 6 , 15,
    (int)(i * ROAD_LINE_SPACE + road_anim), 16);
}

// Determine if a car should be spawned
static int should_spawn_car(void){
  if(game_time % (int)(TICKS_PER_CAR * SPEED_SCALE) != 0) return 0; // Only spawn in intervals
  if(rand()%100 >= CAR_RATE) return 0; // Random chance
  return 1; // Should spawn
}

// Add new car to game
static void spawn_car(void) {
  int i;
  score+=10;
  PORTECLR = 0xFF;
  PORTESET = (score / 40) % 0xFF;
  for (i=0; i < MAX_CARS; i++){
    if(cars[i].alive != 1) {
      int side = (rand()%100 + balance) < 50;
      balance += (side*2-1) * BALANCE_FACTOR;
      start_car(&cars[i], side);
      break;
    }
  }
}

int get_score(void) {
  return score;
}

static bool has_crashed(void){
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

void draw(void) {
  int i;
  draw_road();
  draw_sprite(127 - 16, (int)car_pos, spr_car, 0); //Draw player car

  //Draw cars
  for (i=0; i < MAX_CARS; i++) {
      draw_car(&cars[i]);
  }
}

void tick(void) {
  if(!start){
    start |= get_button(BTN4) | get_button(BTN3);
    return;
  }
  // Increase speed
  speed += ACCELERATION/SPEED_SCALE;

  //Spawning cars
  if(should_spawn_car()) spawn_car();

  //Player controlled car
  float turning = get_button(BTN4) - get_button(BTN3);
  turning *= PLAYER_SPEED + speed * TURN_ACCELERATION;
  turning /= SPEED_SCALE;
  car_pos += turning;
  bound_car();
  debug_var = (int)(SPEED_SCALE);
  //Road animation
  road_anim += speed/SPEED_SCALE;
  while(road_anim >= ROAD_LINE_SPACE) road_anim -= ROAD_LINE_SPACE;

  //Cars
  int i;
  for (i=0; i < MAX_CARS; i++) {
      update_car(&cars[i], (speed + CAR_SPEED) / SPEED_SCALE);
  }

  if(has_crashed()) {
    new_score((Score){{player_name[0], player_name[1], player_name[2], player_name[3]}, score});
    state = STATE_GAMEOVER;
    return;
  }
}
