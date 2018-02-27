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

void draw_ne() {
  display_string(0, "Enter Your Name")
  display_update_debug();
}
void tick_ne(){
  draw_ne();
}
