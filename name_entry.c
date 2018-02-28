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

int selection;
char name[4] = {'A', 'A', 'A', 'A'};
char printbuffer[16];

void draw_ne() {
  display_string(0, "Enter Your Name");

  strcpy(printbuffer, "                ");
  if(selection==5)
    strcpy(printbuffer+11, "^^^^");
  else
    printbuffer[selection*2-1] = '^';
  display_string(1, printbuffer);

  strcpy(printbuffer, "A A A A   [DONE]");
  printbuffer[0] = name[0];
  printbuffer[2] = name[1];
  printbuffer[4] = name[2];
  printbuffer[6] = name[3];
  display_string(2, printbuffer);

  strcpy(printbuffer, "                ");
  if(selection==5)
    strcpy(printbuffer+11, "vvvv");
  else
    printbuffer[selection*2-1] = 'v';
  display_string(3, printbuffer);

  display_update_debug();
}

void tick_ne() {
  selection -= get_button(BTN4);
  selection += get_button(BTN3);
  if(selection < 0) selection += 5;
  if(selection > 4) selection -= 5;

  if(!(get_button(BTN4) || get_button(BTN3))) {
    name[selection] -= get_button(BTN2);
    name[selection] += get_button(BTN1);
    if(name[selection] < 'A') name[selection] += 26;
    if(name[selection] > 'Z') name[selection] -= 26;
  }
  while(get_buttons()){}; //wait until all buttons are released
  draw_ne();
}
