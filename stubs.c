#include "scoreboard.h"
#include "globals.h"

/* Non-Maskable Interrupt; something bad likely happened, so hang */
void _nmi_handler() {
	for(;;);
}

/* This function is called upon reset, before .data and .bss is set up */
void _on_reset() {

}

/* This function is called before main() is called, you can do setup here */
void _on_bootstrap() {
  synchronize_clocks();
  setup_display_pins();
  setup_display_spi();
  display_init();
  setup_inputs();
  init_game_timer();
  init_game();
  init_scoreboard();
  debug_init();
}
