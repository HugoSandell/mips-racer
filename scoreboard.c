#include "scoreboard.h"

static Score scores[10];

void init_scoreboard(void) {
  // TODO: Read data from EEPROM to the scores array
  for(int i=0;i<10;i++)
  {
  printf("score scores[]\n",*score to be entered* );
  }
  while(1) // How many times will this loop?
  {
    if(score_scores[]<= highestscores) /* Syntax error, "score_scores[]" can not be used in a comparison*/
    {
      highestscores = score_scores; /*where is score_scores and highestscores declared?*/
    }
  }
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
