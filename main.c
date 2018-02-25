/*
	main.c
	Created on Feb 16, 2018 by Hugo Sandell

	The entry point of the program
*/
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdlib.h>

void *stdin, *stdout, *stderr; // Workaround for undefined standard library functions

long long game_time = 0;

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
  if(IFS(0) & (1<<12)) {
    IFSCLR(0) = 1 << 12;
    game_time++;
    tick();
  }
}

int init_calls(){
  synchronize_clocks();
  setup_display_pins();
  setup_display_spi();
  display_init();
  setup_inputs();
  init_game_timer();
  init_game();
}

int main() {
PORTE = (PORTE + 1) & 0xF;
  srand(0x9324579); // Initialize randomizer
  init_calls();

	while(1){
    // nop
	}

	return 0;
}
