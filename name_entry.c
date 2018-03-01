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
static char name[4] = {'A', 'A', 'A', 'A'};
static char printbuffer[16];

void draw_ne() {
  display_string(0, "Enter Your Name:");

  strcpy(printbuffer, " A A A A   DONE ");
  printbuffer[1] = name[0];
  printbuffer[3] = name[1];
  printbuffer[5] = name[2];
  printbuffer[7] = name[3];
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

void tick_ne() {
  selection -= get_button(BTN4);
  selection += get_button(BTN3);
  if(selection < 0) selection += 5;
  if(selection > 4) selection -= 5;

  if(selection == 4) {
    if (get_buttons() & 3) {
      state = STATE_GAME;
    }
  } else if(!(get_button(BTN4) || get_button(BTN3))) {
    name[selection] -= get_button(BTN2);
    name[selection] += get_button(BTN1);
    if(name[selection] == ' ' + 1){
      name[selection] = 'A';
    } else if(name[selection] == ' ' - 1){
      name[selection] = 'Z';
    } else if(name[selection] < 'A' || name[selection] > 'Z') {
      name[selection] = ' ';
    }
  }
  while(get_buttons()){}; //wait until all buttons are released
  draw_ne();
}
