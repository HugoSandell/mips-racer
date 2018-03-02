#include <string.h>
#include "chipkitio.h"
#include "u32helpers.h"
#include "states.h"

static void draw_go(){
  char score_str[16];
  display_string(0, "   GAME  OVER   ");
  strcpy(score_str, "Score: ");
  strcpy((char*)(score_str+7), (char*)itoaconv(get_score()));
  display_string(2, score_str);
  display_update_debug();
}

void tick_go(){
  if((get_buttons() & 3) == 3) {
    init_game();
    state = STATE_GAME;
  }
  draw_go();
}
