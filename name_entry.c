/*
  name_entry.c
  Created on Feb 27, 2018 by Hugo Sandell
*/

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdbool.h>  /* Standard header for dealing with boolean values */
#include <math.h>
#include "chipkitio.h"
#include "graphics.h"
#include "states.h"

static int selection;
char player_name[4] = {'A', 'A', 'A', 'A'};
static char printbuffer[16];

void draw_ne() {
  display_string(0, "Enter Your Name:");

  strcpy(printbuffer, " A A A A   DONE ");
  printbuffer[1] = player_name[0];
  printbuffer[3] = player_name[1];
  printbuffer[5] = player_name[2];
  printbuffer[7] = player_name[3];
  if(selection < 4) {
    printbuffer[selection*2] = '[';
    printbuffer[selection*2+2] = ']';
  } else {
    printbuffer[10] = '[';
    printbuffer[15] = ']';
  }
  display_string(2, printbuffer);

  display_update_debug();
}

char* get_name() {
  return player_name;
}

void tick_ne() {
  selection -= get_button(BTN4);
  selection += get_button(BTN3);
  if(selection < 0) selection += 5;
  if(selection > 4) selection -= 5;

  if(selection == 4) {
    if ((get_buttons() & 3) == 3) {
      state = STATE_GAME;
    }
  } else if(!(get_button(BTN4) || get_button(BTN3))) {
    player_name[selection] -= get_button(BTN2);
    player_name[selection] += get_button(BTN1);
    if(player_name[selection] == ' ' + 1){
      player_name[selection] = 'A';
    } else if(player_name[selection] == ' ' - 1){
      player_name[selection] = 'Z';
    } else if(player_name[selection] < 'A' || player_name[selection] > 'Z') {
      player_name[selection] = ' ';
    }
  }
  while(get_buttons()){
    if(selection == 4) {
      if ((get_buttons() & 3) == 3) {
        state = STATE_GAME;
      }
    }
  }; //wait until all buttons are released
  draw_ne();
}
