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
#include "gameover.h"
#include "states.h"

void *stdin, *stdout, *stderr; // Workaround for undefined standard library functions

volatile unsigned game_time = 0;
int return_state = -1; // The state to return to
bool debug_mode = false;
int state;

void idle(int cyc);
void enable_interrupts();
void disable_interrupts();
static void loop();

int main() {
  state = STATE_STARTUP;
  srand(0x9324579); // Initialize randomizer
	while(1){
    idle(1);
    if(IFS(0) & (1<<12)) {
      IFSCLR(0) = 1 << 12;
      TMR3=0;
			loop();
    }
	}
	return 0;
}

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

static void loop(){
	switch (state) {
    case STATE_STARTUP:
      state = STATE_NAME_ENTRY;
    break;
    case STATE_NAME_ENTRY:
      tick_ne();
      break;
		case STATE_GAME:
			game_time++;
			tick();
			draw();
			display_update();
			break;
    case STATE_GAMEOVER:
      tick_go();
      break;
		default:
			debug();
	    if(!get_switch(SW4))state=return_state;
	}
}
