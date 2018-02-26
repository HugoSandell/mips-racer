#ifndef SCOREBOARD_H
#define SCOREBOARD_H

typedef struct {
  char name[4];
  unsigned score;
} Score;

// Initializes the scoreboard and loads data from EEPROM
extern void init_scoreboard(void);

// Returns pointer to scores
extern Score* get_highscores(void);

/* Manages new scores, writes to EEPROM if necessary
   Returns true if the score was inserted into the scoreboard */
extern int new_score(Score);

#endif // SCOREBOARD_H
