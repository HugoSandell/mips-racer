/*
	main.c
	Created on Feb 16, 2018 by Hugo Sandell

	The entry point of the program
*/
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdlib.h>
#include <stdbool.h>
#include "chipkitio.h"

void *stdin, *stdout, *stderr; // Workaround for undefined standard library functions

volatile unsigned game_time = 0;

bool debug_mode = false;

void idle(int cyc){
	int i;
	for(i = 0; i < cyc; i++);
}

void enable_interrupts(){
	__asm__("ei");
}

void disable_interrupts(){
	__asm__("di");
}

void user_isr() {
  return;
}

int main() {
  srand(0x9324579); // Initialize randomizer

	while(1){
    idle(10);
    debug_mode = get_switch(SW4);
    if(IFS(0) & (1<<12)) {
      IFSCLR(0) = 1 << 12;
      TMR3=0;
      if (debug_mode) {
        debug();
      } else{
        game_time++;
        tick();
        draw();
        display_update();
      }
    }
	}

	return 0;
}
