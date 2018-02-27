/*
	debug.c
	Created on Feb 25, 2018 by Hugo Sandell
  Contains functions for debugging
*/

#include <stddef.h>   /* Declarations of integer sizes and the like, part 1 */
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdbool.h>  /* Standard header for dealing with boolean values */
#include "chipkitio.h"

#define DISPLAY_MODE_GAME 0 // display mode for seeing the game
#define DISPLAY_MODE_MEMORY 1 // display mode for printing out memory

// Variables to track
extern volatile unsigned game_time;
extern float speed;
extern int road_anim; // road animation variable
extern int debug_var;


extern void saveinfo(char s[], void * ap, size_t ns);
extern void show_debug_info(void);
int display_mode = 0;

void debug_init() {
    saveinfo("Game Time", (void*)&game_time, sizeof(long long));
    saveinfo("Speed", &speed, sizeof(float));
    saveinfo("Road Anim", &road_anim, sizeof(int));
    saveinfo("Debug", &debug_var, sizeof(int));
}

void debug() {
  display_mode = get_switch(SW3);
  if(get_button(BTN4)) {
    game_time++;
    tick();
  }
  if(display_mode == DISPLAY_MODE_GAME){
    draw();
    display_update();
  } else {
    show_debug_info();
  }
}
