#include "scoreboard.h"

static Score scores[10];

void init_scoreboard(void) {
  // TODO: Read data from EEPROM to scores
}

// Returns pointer to scores
Score* get_highscores(void){
  return scores;
}

/* Manages new scores, writes to EEPROM if necessary
   Returns true if the score was inserted into the scoreboard */
int new_score(Score score){
  // TODO: Decide if score should be inserted into scoreboard
  // If yes, insert score and then write to EEPROM
  return 0;
}
