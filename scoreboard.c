#include "scoreboard.h"

static Score scores[10];

void init_scoreboard(void) {
  // TODO: Read data from EEPROM to the scores array
  int i;
  for(i = 0; i < 10; i++) {
    scores[i] = (Score){"TEST", 1000-i*10};
  }
}

// Returns pointer to scores
Score* get_highscores(void){
  return scores;
}

/* Manages new scores, writes to EEPROM if necessary
   Returns true if the score was inserted into the scoreboard */
int new_score(Score score){
  int i = 0;
  // TODO: Decide if score should be inserted into scoreboard
  // If yes, insert score
  if(score.points > scores[0].points){
    Score tmp[10];
    while (score.points > scores[++i].points);
    while(--i >= 0){
      tmp[i] = scores[i];
    }
  }
  // TODO: Write new array to EEPROM
  return 0;
}
